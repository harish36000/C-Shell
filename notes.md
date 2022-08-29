

# notes

## Specification 1 : Print folder  name and user name

const int -> for ready only in C

- used #define

current direcotry acts as root for shell.

~/Desktop/on/Assignments/A2 

```C
~ //displyed to user as root
```

~/Desktop/on/Assignments/A2/XYZ

```C
~/XYZ //displyed to user as root
```

```shell
~/newdir/test>cd..
~/newdir>cd..
~/> cd..
~> cd..
home/on/Assignment/$
```



```shell
~ = /home/harish/Desktop/on/Assignments/A2
~/abc = /home/harish/Desktop/on/Assignments/A2/abc
```

Working of strtok() ----- Why it ruins your original String

https://stackoverflow.com/questions/21097253/how-does-the-strtok-function-in-c-work



How i am tokenizing

```
cd abc ; cd ..

Tokenizing first time 
- "cd abc"
- "cd .."

Tokenizing second time 
- Command 1
	- "cd" 
	- "abc"
- Command 2
	- "cd"
	- ".."
```

pathMatchUpdate

```
root 	 		= /home/harish/Desktop/on/Assignments/A2 ----> ~ 
root/abc 		= /home/harish/Desktop/on/Assignments/A2/abc ---> ~/abc 
(before root)	= /home/harish/Desktop/on/Assignments ----> /home/harish/Desktop/on/Assignments
```

