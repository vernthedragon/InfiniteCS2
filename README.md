
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

I wrote this way back in 2023 when CS:2 just released. Back then I had quite alot of free time on my hands which I spent creating cheats for multiple games (other than this). Quite the hobby of mine. However, now I am quite lacking in spare time required to even finish the version for CS:2. So enjoy the source code, which is somewhat good and bad at the same time. The source (by now) is definetely outdated and will not run in the latest CS:2 version, however pull-requests are welcome to fix that.

## Some Information

This source has quite the sentimental value to me as it was my hobby during my early teen years to create these cheats. The GUI is very well made and a bit inspired by Neverlose, and has similar binding features to it, albeit the terrible implementation I did.

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

