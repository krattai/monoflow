
THREADS
-------
The first implementation of the concepts of FBP for the PC was written in C, and was 
called THREADS. It used the Borland C compiler. It is described in the first edition of 
my book, "Flow-Based Programming", specifically in the Appendix (see "THREADS API and 
Network Specification" - http://www.jpaulmorrison.com/fbp/threads.shtml ). 

Fibres (fibers in the US)
-------------------------
The code was upgraded to use Win32 and "fibres" - I have put it up on my web site, 
called THREADS_32, and the last version that I worked on is on the "trunk" directory 
on SourceForge. As it is fibre-based, it cannot make use of multiple processors, 
but initial testing suggests that, on the other hand, it is very fast! Although the revised 
code has survived some of its early tests, it obviously needs a lot more testing before it 
becomes "industrial strength". 

I have now changed the name to CppFBP, and in fact this name has started to be used 
in the code, but the book refers to it as THREADS_32, so I will retain that name for the 
zip file, for the time being. 

Note: this implementation has both a static version, and an interpreted version, so components 
have to be packaged two ways - you can see examples of both in the cppfibres directory.

Boost multithreading
----------------------
This version is on the "branches/boostver" directory on SourceForge, and is a little bit 
slower - per processor - than the fibres-based version, but it does use all your machine's 
processors.

It currently has a small number of memory leaks - possibly due to my misuse of the Boost
facilities.  Vicente Botet is looking into this.  Hopefully he will spot something soon!


J. Paul Morrison (Sept. 2013)
