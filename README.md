#### New devs please read Code Example and Optimization Below

### To Do List

- Add Hooks
- Improve SDK


### Broken

- AutoStrafe



## Optimization and Code Example

```C++
//TREAT ALL WARNINGS AS ERROR
//PLEASE KEEP TRACK OF YOUR POINTERS
//PLEASE AVOID LAMBDAS, FUNCTION POINTERS AND RECURSION

//optimized func
//avoid heavy math operations and Cache reused vars
//in Client->cache
float MathFunc(const Vec2& a) { //use const type& for 
//function that require structs as inputs (less memory and stack)
    return Math::Sqrt( a.x * 0.5f ); //AVOID DIVISIONS AS MUCH AS POSSIBLE
    //ex. USE *0.5f instead of / 2.f; (far faster)
    //do not Math functions outside of Math:: (more optimized)
}

void Func() {
    float SomethingA = 0.f;
    { //use brackets for values whom have temp use 
    //so they dont exist throughout the function
        float TempVal = 0.f;
        CS::DoSomething(&TempVal);
        SomethingA *= TempVal;
    }
}

namespace { //use namespaces for functions that are hooks
//or do not require members (ex. int thing = 1;)
    void Function(); 

    inline void DoThing(float a); //for functions  
    //which are short use inline on them
    //to decrease stack usage per function
};

class CSomeClass { //for classes use this naming system
public:
    void Func();
    std::int8_t Member; //do not waste space and only
    //use int sizes u use
};
extern* CSomeClass SomeClass; //use extern because slightly faster
CSomeClass* SomeClass = new CSomeClass();

struct Struct_t { //use this naming for struct that are not 
//related to menu
    void* thing;
};

enum A : std::int8_t { //enum memebers are all caps
//make sure to use ints with bits used
//IF YOU DONT USE MORE THAN 8 BITS DONT USE std::int32_T
    THING = 0
};

```

