#pragma once

#include "ofMain.h"

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdCheck {
private:
	string name;
public:
	phdCheck() { }
	~phdCheck() { }

	void setName(string _name) { name = _name; }
	string getName() { return name; }

	virtual inline int size() { return 0; }

	virtual void setup() { }

	virtual int indexOf(string _value) { return 0; }
	virtual int indexOf(double _value) { return 0; }

	virtual string getType() { return "CHECK"; }

	virtual void getItem(int _index, string * _item) { if(_item != NULL) (*_item) = ""; }
	virtual void getItem(int _index, double * _item) { if(_item != NULL) (*_item) = 0.0; }

	virtual bool check(double _value) { return false; }
	virtual bool check(string _value) { return false; }
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdCheckRange : public phdCheck {
private:
	double min;
	double max;
public:
	phdCheckRange() { min = 0.0; max = 0.0; }
	~phdCheckRange() { }

	inline int size() { return 0; }

	string getType() { return "CHECK_DOUBLE_RANGE"; }

	void setup(double _min, double _max) { min = _min; max = _max; }

	bool check(double _value) {
		if(_value < min || _value > max) return false;
		return true;
	}

	bool check(string _value) {
		check(ofToFloat(_value));
	}
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdCheckDoubleList : public phdCheck {
private:
	vector<double> items;
public:
	phdCheckDoubleList() { }
	~phdCheckDoubleList() { }
	
	inline int size() { return items.size(); }

	string getType() { return "CHECK_DOUBLE_LIST"; }

	virtual int indexOf(string _value) { return indexOf(ofToFloat(_value)); }
	virtual int indexOf(double _value) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i] == _value) return i;
		}
		return -1;
	}

	void getItem(int _index, string * _item) { if(_item != NULL && _index > -1 && _index < size()) (*_item) = ofToString(items[_index]); }
	void getItem(int _index, double * _item) { if(_item != NULL && _index > -1 && _index < size()) (*_item) = items[_index]; }

	void setup(vector<double> _values) { items = _values; }

	bool check(double _value) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i] == _value) return true;
		}
		return false;
	}

	bool check(string _value) {
		return check(ofToFloat(_value));
	}
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdCheckStringList : public phdCheck {
private:
	vector<string> items;
public:
	phdCheckStringList() { }
	~phdCheckStringList() { }

	inline int size() { return items.size(); }

	string getType() { return "CHECK_STRING_LIST"; }

	virtual int indexOf(string _value) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i] == _value) return i;
		}
		return -1;
	}
	virtual int indexOf(double _value) { return indexOf(ofToString(_value)); }

	void getItem(int _index, string * _item) { if(_item != NULL && _index > -1 && _index < size()) (*_item) = items[_index]; }
	void getItem(int _index, double * _item) { if(_item != NULL && _index > -1 && _index < size()) (*_item) = ofToFloat(items[_index]); }

	void setup(vector<string> _values) { items = _values; }

	bool check(double _value) {
		return check(ofToString(_value));
	}

	bool check(string _value) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i] == _value) return true;
		}
		return false;
	}
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdAttributeData {
private:
	phdCheck * check;
public:
	phdAttributeData() { check = NULL; }
	~phdAttributeData() { }

	virtual char getDataType() { return 'U'; }
	//virtual int getDimension() { return 0; }

	virtual void setCheck(phdCheck * _check) { }
	virtual phdCheck * getCheck() { return NULL; }

	virtual void clear() { }
	virtual void copy(phdAttributeData * _src, bool _clear) { }

	virtual string getAsString() { return ""; }
	virtual void setAsString(string _value) { }

	virtual double getAsDouble() { return 0.0; }
	virtual void setAsDouble(double _value) { }

	virtual string getAsString(int _index) { return ""; }
	virtual void setAsString(int _index, string _value) { }

	virtual double getAsDouble(int _index) { return 0.0; }
	virtual void setAsDouble(int _index, double _value) { }
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdAttributeDataString : public phdAttributeData {
private:
	string data;
public:
	phdAttributeDataString() { }
	~phdAttributeDataString() { }

	char getDataType() { return 'S'; }

	void clear() { data = ""; }
	void copy(phdAttributeData * _src) { if(_src != NULL) data = _src->getAsString(); }

	string getAsString() { return data; }
	void setAsString(string _value) {
		phdCheck * _check = getCheck();
		if(_check != NULL && _check->getType() == "CHECK_STRING_LIST") {
			if(_check->check(_value)) data = _value;
		} else {
			data = _value;
		}
	}

	double getAsDouble() { return ofToFloat(getAsString()); }
	void setAsDouble(double _value) { setAsString(ofToString(_value)); }
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdAttributeDataDouble : public phdAttributeData  {
private:
	double data;
public:
	phdAttributeDataDouble() { }
	~phdAttributeDataDouble() { }

	char getDataType() { return 'D'; }
	//int getDimension() { return 1; }

	void clear() { }
	void copy(phdAttributeData * _src, bool _clear) { }

	string getAsString() { return ofToString(getAsDouble()); }
	void setAsString(string _value) { setAsDouble(ofToFloat(_value)); }

	double getAsDouble() { return data; }
	void setAsDouble(double _value) {
		phdCheck * _check = getCheck();
		if(_check != NULL && (_check->getType() == "CHECK_DOUBLE_RANGE" || _check->getType() == "CHECK_DOUBLE_LIST")) {
			if(_check->check(_value)) data = _value;
		} else {
			data = _value;
		}
	}
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdAttribute {
public:
	string name;
	phdAttributeData * data;

	phdAttribute(string _name, char _dataType) {
		name = _name;
		if(_dataType == 'D') {  // double
			data = new phdAttributeDataDouble();
		} else { // string - default
			data = new phdAttributeDataString();
		}
	}

	~phdAttribute() {
		delete data; data = NULL;
	}

	char dataType() { return data->getDataType(); }

	void clear() { data->clear(); }
	void copy(phdAttribute * _src, bool _clear) {
		if(_src != NULL) this->data->copy(_src->data, _clear);
	}

	string getAsString() { return data->getAsString(); }
	void setAsString(string _value) { data->setAsString(_value); }

	double getAsDouble() { return data->getAsDouble(); }
	void setAsDouble(double _value) { data->setAsDouble(_value); }
};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class phdAttributeList {
public:
	vector<phdAttribute*> items;

	phdAttributeList() { }
	~phdAttributeList() {
		freeItems();
	}

	void freeItems() {
		for (vector<phdAttribute*>::iterator i = items.begin(); i != items.end(); i++) {
			delete (*i);
		}
		items.clear();
	}

	phdAttribute * phdAttributeList::operator[](unsigned i) { return items[i]; }
	phdAttribute * phdAttributeList::operator[](string i) { return findByName(i); }

	inline int size() { return items.size(); }

	int indexOf(string _name) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i]->name == _name) return i;
		}
		return -1;
	}

	phdAttribute * findByName(string _name) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i]->name == _name) return items[i];
		}
		return NULL;
	}

	phdAttribute * add(string _name, char _dataType) {
		phdAttribute * _result = findByName(_name);
		if(_result == NULL) {
			_result = new phdAttribute(_name, _dataType);
			items.push_back(_result);
		}
		return _result;
	}

	void setAttribute(int _index, string _value) {
		if(_index > -1 && _index < items.size()) {
			items[_index]->setAsString(_value);
		}
	}

	void setAttribute(int _index, double _value) {
		if(_index > -1 && _index < items.size()) {
			items[_index]->setAsDouble(_value);
		}
	}

	void setAttribute(string _name, string _value) {
		setAttribute(indexOf(_name), _value);
	}
	
	void setAttribute(string _name, double _value) {
		setAttribute(indexOf(_name), _value);
	}

	void getAttributeListNames(vector<string> & _names) {
		for(int i = 0; i < items.size(); i++) {
			_names.push_back(items[i]->name);
		}
	}
};

