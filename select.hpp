#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>
#include <iostream>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const string& s) const = 0;
};

class Select_And : public Select { //the row contains this AND this
	private:
		Select* val1;
		Select* val2;
	public:
		Select_And(Select* select1, Select* select2) {
			val1 = select1; //this will get Select_Contains(&sheet, "Last", "Dole")
			val2 = select2; //this will get Select_Not( Select_Contains(&sheets, "First", "V")
		}
		~Select_And() {
			delete val1;
			delete val2;
		}

		virtual bool select(const Spreadsheet* sheet, int row) const {
			if(val1->select(sheet,row) && val2->select(sheet,row)) { //returns true if the row contain both the select_contains
				return true;
			}
			return false;
		}
};

class Select_Or: public Select { //the row contains this OR this
	private:
		Select* val3;
		Select* val4;
	public:
		Select_Or(Select* select1, Select* select2) {
			val3 = select1; //this get Select_Contains(&sheet, "First", "Amanda")
			val4 = select2; //this get Select_Contains(&sheet, "Last", "on") or Select_Contains(&sheet,"Age", "9"))
		}

		~Select_Or{} {
			delete val3;
			delete val4;
		}

		virtual bool select(const Spreadsheet* sheet, int row) const {
			if(val3->select(sheet,row) || val4->select(sheet,row)) { //if the row has this OR this, return true
				return true;
			}
			return false;
		}
};

class Select_Not : public Select {
	private: 
		Select* val5;
	public:
		Select_Not(Select* select1) {
			val5 = select1;
		}
		~Select_Not{} {
			delete val5;
		}

		virtual bool select(const Spreadsheet* sheet, int row) const {
			bool temp = this->val5->select(sheet,row); //if the row does not contain this string, return true
			if(!temp) {
				return true;
			}
			return false;
		}
};

class Select_Contains : public Select {
	protected: 
		const Spreadsheet* s;
		int column
		string val;
	public:
		Select_Contains(const Spreadsheet* sheet, const string& columnWanted, const string& stringWanted) { //example Select_Contains(&sheets, "First" (columnWanted), "Amanda" (stringWanted)
			column = sheet->get_column_by_name(columnWanted); //column = getcolumn name from the spreadsheet 
			s = sheet; 
			val = stringWanted; 
		}

		virtual bool select(const Spreadsheet* sheet, int row) const {
			return select(row);
		}

		virtual bool select(int row) const {
			if(s->cell_data(row,column).find(val) != string::npos) { //use cell data to get data at the row number and column number. Find string in that row and column. Use string::npos to check if the string val matches the one in the row and column by checking each letter in the string
				return true;
			}
		}
};		
		

#endif //__SELECT_HPP__
