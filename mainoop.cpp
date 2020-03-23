#include <iostream>
#include <list> 
#include <iterator>
#include <algorithm>
#include <fstream> 
using namespace std;

/*
1. ask for main plank size
2. ask for number of items to cut
3. repeapt for all ( ask for size, ask for required number)
*/

#define CW 1.0 // the with of the waste caused by the cutting blade
#define WS 5.0 // defines the minimal dimension of plank not considered waste

class plank;

class rect{
private:
    double w,h,area;
public:
    rect(double w, double h): w(w), h(h), area(w*h) {}

    bool operator==(const rect& b) {
        if(h == b.h && w == b.w )
            return true;
        else
            return false;
    }
    double getW() { return w; } 
    double getH() { return h; } 
    double getArea() { return area; } 

    friend bool operator==(plank a, const plank& b);
};

bool compare( rect& a, rect&b ){
  return a.getArea() > b.getArea();
}

class plank : public rect{
public:
    plank(double w, double h) : rect(w,h) {}
    bool operator==(plank& b) {
        if( this->getH() == b.getH() && this->getW() == b.getW() )
            return true;
        else
            return false;
}
    friend bool operator==(plank a, const plank& b);
};

bool comparep( plank& a, plank&b ){
  return a.getArea() < b.getArea();
}
bool operator==(plank a, const plank& b) {
        if(a.h == b.h && a.w == b.w )
            return true;
        else
            return false;
}

class rectl;

class keszlet{
private:
    list<plank> klist;
public:
    keszlet() {}
    void add(plank p) { klist.push_front(p); }
    void remove(plank p) { klist.remove(p); }
    list<plank> getlist(){ return klist;}

    friend plank;
    friend void cut(rect* r, plank* p, keszlet plist);
    friend bool cuttingselect(rectl &rectlist, keszlet &plist);
};

class rectl{
private:
    list<rect> rlist;
    int n;
public:
    rectl() {}
    void add(rect p) { rlist.push_front(p); }
    void remove(rect p) { rlist.remove(p); }
    int getN() { return n;}
    list<rect> getlist(){ return rlist;}
    friend rectl rect_inp();
    friend bool cuttingselect(rectl &rectlist, keszlet &plist);
};

double inp_verify(){
    double d;
    do{
        cin >> d;
    }while(d <= WS);
    return d;
}

double* readln(string line){
    char delimiter = ' ';
    double* ret = new double[3];
    string t;
    for (auto v: line){
        for(int i=0; i<2; i++){
            t= "";
            while (v != delimiter)
                t += v;
            ret[i] = stod(t);
        }
    }
    return ret;
}

keszlet plank_inp(){  // function for inputing the dimension of a plank (temp)
    keszlet lst;
    ifstream file;
    string tmp;
    double* t = new double[3];
    file.open("stock.csv");
    if (!file) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while( !file.eof() ){
        getline(file,tmp);
        t = readln(tmp);
        for(int i=0; i << int(t[2]) ; i++)
            lst.add({t[0],t[1]});
    }
    lst.getlist().sort(comparep); //    sorting items in a decreasing order based on the area
    file.close();
    return lst;
}

rectl rect_inp(){     // reading in an 'n' number of rectangles to cut and passing them back in a list
    rectl lst;
    ifstream file;
    string tmp;
    double *t;
    file.open("tobecut.csv");
    if (!file) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while( !file.eof() ){
        file >> tmp;
        t = readln(tmp);
        for(int i=0; i << int(t[2]) ; i++)
            lst.add({t[0],t[1]});
    }
    lst.n=int(t[2]);
    lst.rlist.sort(compare); //    sorting items in a decreasing order based on the area
    file.close();
    return lst;
}

void printImg(rect r, plank p){   // deals with visualizing the necessary cuts to the user (temp)
    ofstream file ("steps.csv");
    file << r.getW()+(CW/2) << ", "<< r.getH()+(CW/2)<< endl;
}

void cutdone(rectl &rectlist,rect done, plank p){    // function that contains the necessary operations to be done after cutting out a piece
    rectlist.remove(done);
    printImg(done, p);
}

void cut(rect r, plank p, keszlet &plist){    // function to do the 'cutting' operation, vertical cut first horizontal second
    double w = (p.getW()-r.getW());     // the width of the first new plank
    double a1 = w*p.getH();         // the area of the first new plank  [isn't realy need, could be done inline but its easier to read]
    double h = (p.getH()-r.getH());     // the height of the second new plank
    double a2 = h*r.getW();         // the area of the second new plank

    plist.remove(p);           // removes the plank form the list that we just cut up
    if(w<WS&& h<WS){           // if the plank size = rect size, no new planks needed to be added to the list [needs better solution]
        
    }
    else{   if(w<WS){               // if true only one new plank needs to be added (bottom)
            plist.add({r.getW(),h});
        }else if(h<WS){             // if true only one new plank needs to be added (right)
            plist.add({w,p.getH()});
        }else{                      // if none of them are true, add 2 new planks to list, smaller first, larger second
            if (a1>a2){
                plist.add({w,p.getH()});
                plist.add({r.getW(),h});
            }
            else {
                plist.add({r.getW(),h});
                plist.add({w,p.getH()});
            }
        }
    }   
}

int cutfits(rect r, plank p){ // return 1 if cut fits, -1 if fits but needs rotation and 0 if cut doesn't fit
    if ((r.getW() <= p.getW()) && (r.getH() <= p.getH()))
        return 1;
    else if ((r.getH() <= p.getW()) & (r.getW() <= p.getH()))
            return -1;
        else
            return 0;
}

rect rotateRect(rect r){
    return rect {r.getH(), r.getW()};
}

/// plank to rect [not currently in use] (about equivalent time necessity, up to preference) [not tested]
bool cuttingselect(rectl &rectlist, keszlet &plist){
    list<plank>::iterator itr;
    itr = plist.klist.begin();
    int i = 0;
    while ( (cutfits(rectlist.rlist.front(),*itr) == 0 ) && i<plist.getlist().size()){
        i++;
        itr++;
    }
    if(i>=plist.getlist().size())
        return false;
    else{
        if ( cutfits(rectlist.rlist.front(),*itr) == -1)       // if needs rotation, rotate 90
            rectlist.rlist.front() = rotateRect(rectlist.rlist.front());
        cut(rectlist.rlist.front(),*itr,plist);
        cutdone(rectlist, rectlist.rlist.front(), *itr);
        return true;
    } 
}

void checkWaste(keszlet& p){
    for(auto v: p.getlist()){
        if(v.getH()<WS || v.getW() < WS)
        p.remove(v);
    }
}

int main()
{
    keszlet p;
    keszlet waste;
    p = plank_inp();
    rectl rectlist = rect_inp();
    rectl uns; 
    checkWaste(p);
    while(!p.getlist().empty() && !rectlist.getlist().empty()){     // amig van vágando elem es vághato lap
        if(!cuttingselect(rectlist, p)){   // find a plank that fits the first element of rectlist
            uns.add(rectlist.getlist().front());
            rectlist.remove(rectlist.getlist().front());
        }
    }      
}
