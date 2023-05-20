#include"val.h"
#include <string> 


//DONE?
Value Value::SGthan(const Value& oper) const{//Works
        string oper_1 = "";
        string oper_2 = "";
        if(GetType() == oper.GetType()){
            if(IsInt()){
                return Value(Itemp > oper.GetInt());
            }
            else if(IsReal()){
                return Value(Rtemp > oper.GetReal());
            }
            else if(IsString()){
                return Value(Stemp > oper.GetString());
            }
            else return Value();
        }
        else if(IsInt() && oper.IsReal()){
          
                oper_1 = to_string(GetInt());
                oper_2 = to_string(oper.GetReal());

            //return Value((float)Itemp < oper.GetReal());
            return Value(oper_1 > oper_2);
        }
        else if(IsReal() && oper.IsInt()){
       
                oper_1 = to_string(GetReal());
                oper_2 = to_string(GetInt());

            //return Value(Rtemp < (float)oper.GetInt());
            return Value(oper_1 > oper_2);
        }
        else if(IsString() && oper.IsInt()){//If op 1 is a string
            //Try converting the other to a string

        
            oper_2 = to_string(GetInt());

            return Value(GetString() > oper_2);
        }
        else if(IsString() && oper.IsReal()){//If op 2 is a string
            //Try converting the other to a string
      
            oper_2 = to_string(oper.GetReal());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
            
    
            return Value(GetString() > oper_2);
        }
        else if(IsInt() && oper.IsString()){
          
            oper_1 = to_string(GetInt());
        
            return Value(oper_1 > oper.GetString());
        }
        else if(IsReal() && oper.IsString()){
         
            oper_1 = to_string(GetReal());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
          

            return Value(oper_1 > oper.GetString());
        }
        else{
            return Value();
        }
    
}


//DONE?
Value Value::SEqual(const Value& oper) const{//Works
        string oper_1 = "";
        string oper_2 = "";
        if(GetType() == oper.GetType()){//Might not work
            if(IsInt()){
                return Value(Itemp == oper.GetInt());
            }
            else if(IsReal()){
                return Value(Rtemp == oper.GetReal());
            }
            else if(IsString()){
                return Value(Stemp == oper.GetString());
            }
            else return Value();
        }
        else if(IsInt() && oper.IsReal()){
          
                oper_1 = to_string(GetInt());
                oper_2 = to_string(oper.GetReal());

            //return Value((float)Itemp < oper.GetReal());
            return Value(oper_1 == oper_2);
        }
        else if(IsReal() && oper.IsInt()){
       
                oper_1 = to_string(GetReal());
                oper_2 = to_string(GetInt());

            //return Value(Rtemp < (float)oper.GetInt());
            return Value(oper_1 == oper_2);
        }
        else if(IsString() && oper.IsInt()){//If op 1 is a string
            //Try converting the other to a string

        
            oper_2 = to_string(GetInt());

            return Value(GetString() == oper_2);
        }
        else if(IsString() && oper.IsReal()){//If op 2 is a string
            //Try converting the other to a string
      
            oper_2 = to_string(oper.GetReal());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
            
    
            return Value(GetString() == oper_2);
        }
        else if(IsInt() && oper.IsString()){
          
            oper_1 = to_string(GetInt());
        
            return Value(oper_1 == oper.GetString());
        }
        else if(IsReal() && oper.IsString()){
         
            oper_1 = to_string(GetReal());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
          

            return Value(oper_1 == oper.GetString());
        }
        else{
            return Value();
        }
}

string repeatString(string str, int times){//Fix maybe with const
        string repeatedStr = "";
        for (int i = 0; i < times; i++) {
            repeatedStr += str;
        }
        return repeatedStr;
}




Value Value::Repeat(const Value& oper) const{
    string oper_1 = "";
    string oper_2 = "";
    //int oper_1_1;
    int oper_2_2;
    if(GetType() == oper.GetType()){
        if(IsString()){
            try{
                oper_2_2 = stoi(oper.GetString());

            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(GetString(),oper_2_2));

        }
        else if(IsInt()){
            try{
                oper_1 = to_string(GetInt());
                
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,oper.GetInt()));
        }
        else if(IsReal()){
            try{
                oper_1 = to_string(GetReal());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,(int)oper.GetReal()));//Might be wrong
        }
        else return Value();
    }
    else if(IsInt()){
        if(oper.IsReal()){
            try{
                oper_1 = to_string(GetInt());
                
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,(int)oper.GetReal()));
        }
        else if(oper.IsString()){
            try{
                oper_1 = to_string(GetInt());
                oper_2_2 = stoi(oper.GetString());

            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,oper_2_2));
        }
        else return Value();
        
    }
    else if(IsReal()){
        if(oper.IsInt()){
           try{
                oper_1 = to_string(GetReal());
                
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,oper.GetInt()));
        }
        else if(oper.IsString()){
            try{
                oper_1 = to_string(GetReal());
                oper_2_2 = stoi(oper.GetString());

            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(repeatString(oper_1,oper_2_2));
        }
        else return Value();

    }
    else if(IsString()){
        if(oper.IsInt()){
            return Value(repeatString(GetString(),oper.GetInt()));
        }
        else if(oper.IsReal()){
            return Value(repeatString(GetString(),(int)oper.GetReal()));
        }
        else return Value();
    }
    return Value();
}

//Done?
Value Value::operator<(const Value& op) const{
    double oper;
    double oper_2;
    if(GetType() == op.GetType()){
        if(IsInt()){
            return Value(Itemp<op.GetInt());
        }
        else if(IsReal()){
            return Value(Rtemp<op.GetReal());
        }
        else if(IsString()){
             try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper<oper_2);//MIGHT BE UNTRUE
        }
        return Value();
    }
    else if(IsInt()){
        if(op.IsReal()){
            return Value(Itemp<op.GetReal());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Itemp<oper_2);
        }
        return Value();//Might be untrue
        
    }
    else if(IsString()){
        if(op.IsInt()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper<op.GetInt());
        }
        else if(op.IsReal()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper<op.GetReal());
        }
        return Value();
    }
    return Value();
}

//DONE?
Value Value::operator>(const Value& op) const{
    double oper;
    double oper_2;
    if(GetType() == op.GetType()){
        if(IsInt()){
            return Value(Itemp>op.GetInt());
        }
        else if(IsReal()){
            return Value(Rtemp>op.GetReal());
        }
        else if(IsString()){
             try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper>oper_2);//MIGHT BE UNTRUE
        }
        return Value();
    }
    else if(IsInt()){
        if(op.IsReal()){
            return Value(Itemp>op.GetReal());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Itemp>oper_2);
        }
        return Value();//Might be untrue
        
    }
    else if(IsString()){
        if(op.IsInt()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper>op.GetInt());
        }
        else if(op.IsReal()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper>op.GetReal());
        }
        return Value();
    }
    return Value();
}


//DONE?
Value Value::operator/(const Value& op) const{
    double oper;
    double oper_2;

    if((op.IsInt() && op.GetInt() == 0) || (op.IsReal() && op.GetReal() == 0.0)){
            cout<<"Illegal operand type for the operation."<<endl;
            return Value();
    }
    if(GetType() == op.GetType()){
        
        if(IsInt()){
            return Value(Itemp/op.GetInt());
        }
        else if(IsReal()){
            return Value(Rtemp/op.GetReal());
        }
        else if(IsString()){
             try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper/oper_2);//MIGHT BE UNTRUE
        }
        return Value();
    }
    else if(IsInt()){
        if(op.IsReal()){
            return Value(Itemp + op.GetReal());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Itemp + oper_2);
        }
        return Value();//Might be untrue
        
    }
    else if(IsReal()){
        if(op.IsInt()){
            return Value(Rtemp/op.GetInt());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Rtemp/oper_2);
        }
        return Value();//Might be untrue
    }
    else if(IsString()){
        if(op.IsInt()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper/op.GetInt());
        }
        else if(op.IsReal()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper/op.GetReal());
        }
        return Value();
    }
    return Value();
    

}


//Done?
Value Value::operator+(const Value& op) const{

    double oper;
    double oper_2;

    if(GetType() == op.GetType()){
        if(IsInt()){
            return Value(Itemp + op.GetInt());
        }
        else if(IsReal()){
            return Value(Rtemp + op.GetReal());
        }
        else if(IsString()){
            try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper + oper_2);//MIGHT BE UNTRUE
        }
    }
    else if(IsInt()){
        if(op.IsReal()){
            return Value(Itemp + op.GetReal());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Itemp + oper_2);
        }
        return Value();//Might be untrue
        
    }
    else if(IsReal()){
        if(op.IsInt()){
            return Value(Rtemp + op.GetInt());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Rtemp + oper_2);
        }
        return Value();//Might be untrue
    }
    else if(IsString()){
        if(op.IsInt()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper + op.GetInt());
        }
        else if(op.IsReal()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper + op.GetReal());
        }
        return Value();
    }
    return Value();
    
}

//Done?
Value Value::operator-(const Value& op) const{
    double oper;
    double oper_2;

    if(GetType() == op.GetType()){
        if(IsInt()){
            return Value(Itemp - op.GetInt());
        }
        else if(IsReal()){
            return Value(Rtemp - op.GetReal());
        }
        else if(IsString()){
            try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper - oper_2);//MIGHT BE UNTRUE
        }
    }
    else if(IsInt()){
        if(op.IsReal()){
            return Value(Itemp - op.GetReal());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Itemp - oper_2);
        }
        return Value();//Might be untrue
        
    }
    else if(IsReal()){
        if(op.IsInt()){
            return Value(Rtemp - op.GetInt());
        }
        else if(op.IsString()){
            try{
            oper_2 = stod(op.GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(Rtemp - oper_2);
        }
        return Value();//Might be untrue
    }
    else if(IsString()){
        if(op.IsInt()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper - op.GetInt());
        }
        else if(op.IsReal()){
            try{
            oper = stod(GetString());
            }
            catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
            }
            return Value(oper - op.GetReal());
        }
        return Value();
    } 
    return Value();
    
}


//DONE!!!!!!!!!
Value Value::operator*(const Value& op) const{//case 1
        /*if(IsInt() && op.IsInt()){
            //return Value(Itemp * op.GetInt());

        }
        else if(IsReal() && op.IsReal()){
            return Value(Rtemp * op.GetReal());
        }
        else if(IsReal() && )
        else if(IsErr()){
            return;


        }
        else if(op.IsErr()){
            return;

        }
        else{
            return Value();
        }
        */
        double oper;
        double oper_2;
        if(GetType() == op.GetType()){
            //If they have the same type... then we can just check if one is real or int or error
            if(IsInt()){//Handles both int
                return Value(Itemp * op.GetInt());
            }
            else if(IsReal()){//Handles both real
                return Value(Rtemp * op.GetReal());
            }
            
            else if(IsString()){//handles both strings
                try{
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());
                    
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(oper * oper_2);
                
            }
            else{
                return Value();
            }

        }
        else if(IsReal() && op.IsInt()){//Handles op 1 as real and op 2 as int 
            return Value(Rtemp * (float)op.GetInt());
        }
            
        else if(IsInt() && op.IsReal()){//handles op 1 as int and op 2 as real
            return Value((float)Itemp * op.GetReal());
        }
        else if(IsInt() && op.IsString()){//Needs fixing
            try{
                oper = stod(op.GetString());
                

            }
            catch(...){//FIX
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();

            }
            return Value((float)GetInt() * oper);//FIX
        }
        else if(IsReal() && op.IsString()){//Needs fixing
    
            try{
                oper = stod(op.GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(GetReal() * oper);//FIX
        }
        else if(IsString() && op.IsInt()){
            try{
                oper = stod(GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(oper * (float)op.GetInt());
        }
        else if(IsString() && op.IsReal()){
            try{
                oper = stod(GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            return Value(oper * op.GetReal());
        }
            


       
        else
        return Value();//In case none of the if statements work!


    }



    //DONE!!!!!!!!!
    Value Value::operator==(const Value& op) const{//Case2
        double oper_1;
        double oper_2;
        if(GetType() == op.GetType()){
            if(IsString()){
                try{
                    oper_1 = stod(GetString());
                    oper_2 = stod(op.GetString());


                }
                catch(...){
                    cout<<"Invalid conversion from string to double."<<endl;
                    return Value();
                }
                return Value(oper_1 == oper_2);
            }
            else if(IsInt()){
                return Value(Itemp == op.GetInt());
            }
            else if(IsReal()){
                return Value(Rtemp == op.GetReal());
            }
            else{
                return Value();
            }

        }
        else if(IsReal() && op.IsInt()){//Handles op 1 as real and op 2 as int 
            return Value(Rtemp == (float)op.GetInt());
        }
            
        else if(IsInt() && op.IsReal()){//handles op 1 as int and op 2 as real
            return Value((float)Itemp == op.GetReal());
        }
        else if(IsString() && op.IsInt()){
            try{
                oper_1 = stod(GetString());
            }
            catch(...){
                cout<<"Invalid conversion from string to double."<<endl;
                return Value();
            }
            return Value(oper_1 == (float)op.GetInt());
            
            
        }
        else if(IsString() && op.IsReal()){
            try{
                oper_1 = stod(GetString());
            }
            catch(...){
                cout<<"Invalid conversion from string to double."<<endl;
                return Value();
            }
            return Value(oper_1 == op.GetReal());
        }
        else if(IsInt() && op.IsString()){
            try{
                oper_2 = stod(op.GetString());
            }
            catch(...){
                cout<<"Invalid conversion from string to double."<<endl;
                return Value();
            }
            return Value((float)GetInt() == oper_2);
        }
        else if(IsReal() && op.IsString()){
            try{
                oper_2 = stod(op.GetString());
            }
            catch(...){
                cout<<"Invalid conversion from string to double."<<endl;
                return Value();
            }
            return Value(GetReal() == oper_2);
        }
        return Value();

    }




    //DONE!!!!!!!!!
    Value Value::operator^(const Value& oper) const{//Case 3
        if(GetType() == oper.GetType()){
            if(IsInt()){
                return Value(pow((double)Itemp,(double)oper.GetInt()));
            }
            if(IsReal()){
                return Value(pow((double)Rtemp, oper.GetReal()));
            }
            else{
                return Value();
            }

        }
        else if (IsReal() && oper.IsInt()) return Value(pow(Rtemp, (double)oper.GetInt()));
        else if (IsInt() && oper.IsReal()) return Value(pow((double)Itemp, oper.GetReal()));
        else if (IsString() || oper.IsString()) return Value();
        
        return Value(); //Error because if the types dont equal then u cant do power
        

    }
    Value Value::Catenate(const Value& oper) const{//Case 4, can catenate everything except error
        string oper_1 = "";
        string oper_2 = "";
        if(GetType() == oper.GetType()){
            if(IsString()){
                return Value(Stemp + oper.GetString());
            }
            else if(IsInt()){
            
                oper_1 = to_string(GetInt());
                oper_2 = to_string(oper.GetInt());
                

                return Value(oper_1 + oper_2);
                
            }
            else if(IsReal()){
                
                oper_1 = to_string(GetReal());
                oper_2 = to_string(oper.GetReal());
                oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
                oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
                oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
                oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);



                return Value(oper_1 + oper_2);
            }
            else return Value();
        }
        else if(IsInt() && oper.IsReal()){
            
                oper_1 = to_string(GetInt());
                oper_2 = to_string(GetReal());
                oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
                oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
                oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
                oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
           
            return Value(oper_1 + oper_2);
            

        }
        else if(IsInt() && oper.IsString()){
          
            oper_1 = to_string(GetInt());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
 
            return Value(oper_1 + oper.GetString());
        }
        else if(IsReal() && oper.IsInt()){
            oper_1 = to_string(GetReal());
            oper_2 = to_string(GetInt());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
            
            return Value(oper_1 + oper_2);
        }
        else if(IsReal() && oper.IsString()){
            
            oper_1 = to_string(GetReal());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);

            return Value(oper_1 + oper.GetString());
        }
        else if(IsString() && oper.IsInt()){
        
            oper_2 = to_string(oper.GetInt());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);

            return Value(GetString() + oper_2);
       
        }
        else if(IsString() && oper.IsReal()){
            oper_2 = to_string(oper.GetReal());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);


            return Value(Stemp + oper_2);
        }
        else return Value();

    }

    Value Value::SLthan(const Value& oper) const{//Case 5
        string oper_1 = "";
        string oper_2 = "";
        if(GetType() == oper.GetType()){
            if(IsInt()){
                return Value(Itemp < oper.GetInt());
            }
            else if(IsReal()){
                return Value(Rtemp < oper.GetReal());
            }
            else if(IsString()){
                return Value(Stemp < oper.GetString());
            }
            else return Value();
        }
        else if(IsInt() && oper.IsReal()){
          
                oper_1 = to_string(GetInt());
                oper_2 = to_string(oper.GetReal());

            //return Value((float)Itemp < oper.GetReal());
            return Value(oper_1 < oper_2);
        }
        else if(IsReal() && oper.IsInt()){
       
                oper_1 = to_string(GetReal());
                oper_2 = to_string(GetInt());

            //return Value(Rtemp < (float)oper.GetInt());
            return Value(oper_1 < oper_2);
        }
        else if(IsString() && oper.IsInt()){//If op 1 is a string
            //Try converting the other to a string

        
            oper_2 = to_string(GetInt());

            return Value(GetString() < oper_2);
        }
        else if(IsString() && oper.IsReal()){//If op 2 is a string
            //Try converting the other to a string
      
            oper_2 = to_string(oper.GetReal());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
            
    
            return Value(GetString() < oper_2);
        }
        else if(IsInt() && oper.IsString()){
          
            oper_1 = to_string(GetInt());
        
            return Value(oper_1 < oper.GetString());
        }
        else if(IsReal() && oper.IsString()){
         
            oper_1 = to_string(GetReal());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
          

            return Value(oper_1 < oper.GetString());
        }
        else{
            return Value();
        }
        
    }
