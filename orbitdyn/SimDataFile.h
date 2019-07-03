#pragma once

#include <hdf5\h5Cpp.h>
#include <hdf5\H5PacketTable.h>

#if _DEBUG
#pragma comment(lib,"libhdf5_D.lib")
#pragma comment(lib,"libhdf5_hl_D.lib")
#pragma comment(lib,"libhdf5_cpp_D.lib")
#pragma comment(lib,"libhdf5_hl_cpp_D.lib")
#pragma comment(lib,"libszip.lib")
#pragma comment(lib,"libzlib.lib")
#else
#pragma comment(lib,"libhdf5.lib")
#pragma comment(lib,"libhdf5_hl.lib")
#pragma comment(lib,"libhdf5_cpp.lib")
#pragma comment(lib,"libhdf5_hl_cpp.lib")
#pragma comment(lib,"libszip.lib")
#pragma comment(lib,"libzlib.lib")
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
		for(size_t i=0;i<vpt.size();i++)
		{
			delete vpt[i];
		}
		delete hf;
	}

	void Open(std::string filename)	{
		hf = new H5::H5File(filename.c_str(),H5F_ACC_TRUNC);
	}

	void Close() {
		for(size_t i=0;i<vpt.size();i++)
		{
			delete vpt[i];
		}
		delete hf;
	}

	void AddVarDouble(std::string varname,int row,int col,void * pdata)	{
		hsize_t dims[2] = {row,col};
		hid_t  datatype = H5Tarray_create(H5T_NATIVE_DOUBLE, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar,varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(),tmpchar,datatype,1,-1);
		vpt.push_back(pt);
		vdata.push_back(pdata);
		vname.push_back(varname);
	}

	void AddVarFloat(std::string varname, int row, int col, void * pdata) {
		hsize_t dims[2] = { row,col };
		hid_t  datatype = H5Tarray_create(H5T_NATIVE_FLOAT, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar, varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(), tmpchar, datatype, 1, -1);
		vpt.push_back(pt);
		vdata.push_back(pdata);
		vname.push_back(varname);
	}

	void AddVarInt(std::string varname, int row, int col, void * pdata) {
		hsize_t dims[2] = { row,col };
		hid_t  datatype = H5Tarray_create(H5T_NATIVE_INT, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar, varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(), tmpchar, datatype, 1, -1);
		vpt.push_back(pt);
		vdata.push_back(pdata);
		vname.push_back(varname);
	}

	void AddVarByte(std::string varname, int row, int col, void * pdata) {
		hsize_t dims[2] = { row,col };
		hid_t  datatype = H5Tarray_create(H5T_NATIVE_UCHAR, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar, varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(), tmpchar, datatype, 1, -1);
		vpt.push_back(pt);
		vdata.push_back(pdata);
		vname.push_back(varname);
	}

	void Addarmamat(std::string varname,mat& m){
		hsize_t dims[2] = {m.n_rows,m.n_cols};
		hid_t  datatype = H5Tarray_create(H5T_NATIVE_DOUBLE, 2, dims);
		char tmpchar[128];
		strcpy(tmpchar,varname.c_str());
		FL_PacketTable *pt = new FL_PacketTable(hf->getId(),tmpchar,datatype,1,-1);
		vpt.push_back(pt);
		vdata.push_back(m.memptr());
		vname.push_back(varname);
	}

	void Append(){
		for(size_t i=0;i<vpt.size();i++)
		{
			vpt[i]->AppendPacket(vdata[i]);
		}
	}

private:
	// File
	H5::H5File* hf;
	// table list
	std::vector<FL_PacketTable *> vpt;
	// data pointer list
	std::vector<void*> vdata;
	// data name list
	std::vector<std::string> vname;
	// three list are one by one
};

/** Example:
/code

	double att[3],datt[3];
	
	SimDataFile file1;
	
	file1.Open("filename.h5");
	file1.AddVarDouble("att",1,3,&att);
	file1.AddVarDouble("datt",1,3,&datt);
	
	while(����ѭ��)
	{
		// ���з�����㣬�ı�att��datt��ֵ
		
		file1.Append();
	}
	file1.close();

/endcode
*/
	