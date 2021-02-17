#pragma once

#include <hdf5\h5Cpp.h>
#include <hdf5\H5PacketTable.h>

#ifdef _WIN32
	#ifdef _WIN64
		#pragma comment(lib,"libhdf5.lib")
		#pragma comment(lib,"libhdf5_hl.lib")
		#pragma comment(lib,"libhdf5_cpp.lib")
		#pragma comment(lib,"libhdf5_hl_cpp.lib")
	#else
			#if _DEBUG
			#pragma comment(lib,"libhdf5_D.lib")
			#pragma comment(lib,"libhdf5_hl_D.lib")
			#pragma comment(lib,"libhdf5_cpp_D.lib")
			#pragma comment(lib,"libhdf5_hl_cpp_D.lib")
			#pragma comment(lib,"libszip_D.lib")
			#pragma comment(lib,"libzlib_D.lib")
		#else
			#pragma comment(lib,"libhdf5.lib")
			#pragma comment(lib,"libhdf5_hl.lib")
			#pragma comment(lib,"libhdf5_cpp.lib")
			#pragma comment(lib,"libhdf5_hl_cpp.lib")
			#pragma comment(lib,"libszip.lib")
			#pragma comment(lib,"libzlib.lib")
		#endif
	#endif
#endif


#include <armadillo_BLAS_LAPACK.h>

class SimDataFile
{
public:
	SimDataFile():hf(NULL){}
	SimDataFile(std::string filename){
		hf = new H5::H5File(filename.c_str(),H5F_ACC_TRUNC);
	}
	~SimDataFile(){
		Close();
	}

	void Open(std::string filename)	{
		hf = new H5::H5File(filename.c_str(),H5F_ACC_TRUNC);
	}

	void Close() {
		for(size_t i=0;i<vpt.size();i++)
		{
			if(vpt[i]!=NULL)
				delete vpt[i];
			vpt[i] = NULL;
		}		
		if(hf!=NULL)
			delete hf;
		hf = NULL;
	}

	void AddVar(std::string varname,hid_t numtype,int row,int col,void * pdata)	{
		hsize_t dims[2] = {row,col};
		hid_t  datatype = H5Tarray_create(numtype, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar,varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(),tmpchar,datatype,1,-1);
		vpt.push_back(pt);
		vdata.push_back(pdata);
		vname.push_back(varname);
	}

	void AddVarDouble(std::string varname,int row,int col,void * pdata)	{
		AddVar(varname,H5T_NATIVE_DOUBLE,row,col,pdata);
	}

	void AddVarFloat(std::string varname,int row,int col,void * pdata)	{
		AddVar(varname,H5T_NATIVE_FLOAT,row,col,pdata);
	}

	void AddVarInt(std::string varname,int row,int col,void * pdata)	{
		AddVar(varname,H5T_NATIVE_INT,row,col,pdata);
	}

	void AddVarUint(std::string varname,int row,int col,void * pdata)	{
		AddVar(varname,H5T_NATIVE_UINT,row,col,pdata);
	}

	void AddVarByte(std::string varname,int row,int col,void * pdata)	{
		AddVar(varname,H5T_NATIVE_UCHAR,row,col,pdata);
	}

	void Addarmamat(std::string varname,arma::mat& m){
		AddVarDouble(varname,m.n_rows,m.n_cols,m.memptr());
	}

	void Append(){
		for(size_t i=0;i<vpt.size();i++)
		{
			vpt[i]->AppendPacket(vdata[i]);
		}
	}

	void AddData(std::string varname,hid_t numtype,int row,int col,void * pdata)
	{
		hid_t       dataset;         /* file and dataset handles */
		hid_t       datatype, dataspace;   /* handles */
		hsize_t     dimsf[2];              /* dataset dimensions */
		herr_t      status;
		/*
		 * Describe the size of the array and create the data space for fixed
		 * size dataset.
		 */
		dimsf[0] = row;
		dimsf[1] = col;
		dataspace = H5Screate_simple(2, dimsf, NULL);

		/*
		 * Define datatype for the data in the file.
		 * We will store little endian INT numbers.
		 */
		datatype = H5Tcopy(numtype);
		status = H5Tset_order(datatype, H5T_ORDER_LE);

		/*
		 * Create a new dataset within the file using defined dataspace and
		 * datatype and default dataset creation properties.
		 */
		dataset = H5Dcreate2(hf->getId(), varname.c_str(), datatype, dataspace,
				H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

		/*
		 * Write the data to the dataset using default transfer properties.
		 */
		status = H5Dwrite(dataset, numtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, pdata);
	}

private:
	// 文件指针
	H5::H5File* hf;
	// 数据包列表
	std::vector<FL_PacketTable *> vpt;
	// 数据指针列表
	std::vector<void*> vdata;
	// 数据名列表
	std::vector<std::string> vname;
	// 上面三个列表为一一对应关系
};

#define AddSingleUint(file,varname)			file.AddVarUint(#varname,1,1,&varname)
#define AddSingleInt(file,varname)			file.AddVarInt(#varname,1,1,&varname)
#define AddSingleFloat(file,varname)		file.AddVarFloat(#varname,1,1,&varname)
#define AddSingleDouble(file,varname)		file.AddVarDouble(#varname,1,1,&varname)
#define AddMatUint(file,varname,row,col)	file.AddVarUint(#varname,row,col,varname)
#define AddMatInt(file,varname,row,col)		file.AddVarInt(#varname,row,col,varname)
#define AddMatFloat(file,varname,row,col)	file.AddVarFloat(#varname,row,col,varname)
#define AddMatDouble(file,varname,row,col)	file.AddVarDouble(#varname,row,col,varname)

/** Example:
/code

	double att[3],datt[3];
	
	SimDataFile file1;
	
	file1.Open("filename.h5");
	file1.AddVarDouble("att",1,3,&att);
	file1.AddVarDouble("datt",1,3,&datt);
	
	while(仿真循环)
	{
		// 进行仿真计算，改变att和datt的值
		
		file1.Append();
	}
	file1.close();

/endcode
*/
	