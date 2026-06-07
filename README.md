
<p align="center">
 <h2 align="center">Infinite CS:2 Client</h2>
 <p align="center"></p>
</p>

<p align="center">
 
 <a href="https://en.wikipedia.org/wiki/C%2B%2B">
      <img src="https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor"/>
    </a>
    <a href="https://en.wikipedia.org/wiki/Microsoft_Windows">
      <img src="https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor"/>
    </a>
 
 <a href="https://en.wikipedia.org/wiki/Cheating_in_online_games">
      <img src="https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor"/>
    </a>
 
 <a href="https://en.wikipedia.org/wiki/Cheating_in_online_games">
      <img src="https://img.shields.io/badge/License-MIT-e49eff?style=for-the-badge&logo=appveyor"/>
    </a>
 
 
    
  </p>

## Background

This project was developed in 2023 shortly following the release of Counter-Strike 2, undertaken as a personal research endeavour into Windows internals, reverse engineering, and low-level game engine analysis during a period of significant free time. The primary objective was to gain a deep practical understanding of how modern game engines manage memory, how internal DLL injection operates at a low level within a live process, and how anticheat systems identify and respond to such techniques — knowledge that is directly applicable to the development and hardening of anticheat systems themselves.
The project is now largely outdated relative to the current state of CS2, as the game has undergone significant updates since development ceased, and the source code is no longer functional against the latest version. Pull requests to update compatibility are welcome. The GUI implementation, while self-admittedly imperfect in certain areas, was heavily inspired by professional-grade cheat clients and served as a meaningful exercise in designing responsive, feature-rich interfaces within a constrained, performance-critical environment.


### Most of the source is out of date

![Reference](https://raw.githubusercontent.com/vernthedragon/InfiniteCS2/refs/heads/main/ref1.png)
![Reference](https://raw.githubusercontent.com/vernthedragon/InfiniteCS2/refs/heads/main/ref2.png)
![Reference](https://raw.githubusercontent.com/vernthedragon/InfiniteCS2/refs/heads/main/ref3.png)
![Reference](https://raw.githubusercontent.com/vernthedragon/InfiniteCS2/refs/heads/main/ref4.png)


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

