#ifndef __GK_BASE_MACROS__
#define __GK_BASE_MACROS__

#define GET(x,y) private:x y;public:inline x get##y(){return y;};
#define SET(x,y) public:inline void set##y(x a){y = a;};
#define GET_SET(x,y) GET(x,y) SET(x,y)

#define GET_STATIC(x,y) private:static x y;public:inline x get##y(){return y;};
#define GET_SET_STATIC(x,y) GET_STATIC(x,y) SET(x,y)

#define PRINT(y) public:inline void print##y(){cout<<"##y:"<<y<<endl;};
#define GET_SET_PRINT(x,y) private:x y; GET(x,y) SET(x,y) PRINT(y)

#endif