#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
using namespace std;

class data_list
{
public:
	enum SIMDATATYPE{dlBOOL,dlINT,dlFLOAT,dlDOUBLE};
	typedef struct{
		std::string name;
		std::string unit;
		SIMDATATYPE type;
		void *      p;
	}DataElement;

	data_list(){}
	~data_list(){
		_list.clear();
	}
	void append(std::string n,void* p,SIMDATATYPE t){
		DataElement e;
		e.name = n;
		e.type = t;
		e.p = p;
		_list.push_back(e);
	}
	void append(std::string n,void* p,SIMDATATYPE t,std::string u){
		DataElement e;
		e.name = n;
		e.unit = u;
		e.type = t;
		e.p = p;
		_list.push_back(e);
	}
	void append(DataElement& de){
		_list.push_back(de);
	}
	bool print_title(ostream& os){
		if(_list.empty()) return false;
		os << "%\t";
		int i=1;
		for(std::list<DataElement>::iterator it=_list.begin();it!=_list.end();it++)
		{
			os << i++ << "-" << it->name;
			if(it->unit.empty())
				os << "\t";
			else
				os << "(" << it->unit << ")\t";
		}
		os << "\n";
		return true;
	}
	bool print_row(ostream& os){
		if(_list.empty()){
			return false;
		}
		for(std::list<DataElement>::iterator it=_list.begin();it!=_list.end();it++)
		{
			switch(it->type){
			case dlBOOL:
				os << std::setw(14) << std::showpos;
				os << *((bool*)(it->p));
				break;
			case dlINT:
				os << std::setw(14) << std::showpos;
				os << *((int*)(it->p));
				break;
			case dlFLOAT:  // TODO: 设置精度、宽度等参数
				os.precision(6);
				os << std::scientific << std::showpos;
				os << *((float*)(it->p));
				break;
			case dlDOUBLE:
				os.precision(12);
				os << std::scientific << std::showpos;
				os << *((double*)(it->p));
				break;
			default:
				os << "Not_Defined_Type";
			}
			os << "\t";
		}
		os << "\n";
		return true;
	}
private:
	std::list<DataElement> _list;
};

class data_file
{
public:
	data_file(const std::string filename):wheader(false){
		_fs.open(filename,ios::out);
	}
	data_file(const char* filename):wheader(false){
		_fs.open(filename,ios::out);
	}
	~data_file(){
		_fs.close();
	}
	void AddCol(data_list::DataElement& de){
		_dl.append(de);
	}
	void SetList(data_list& dl){
		_dl = dl;
	}
	void WriteRow(){
		if(!wheader) {
			_dl.print_title(_fs);
			wheader = true;
		}
		_dl.print_row(_fs);
	}
private:
	fstream _fs;
	data_list _dl;
	bool wheader;
};