QUESTION 1: 
Another encoding of Unicode is UTF-32, which encodes all Unicode code points in 4 bytes. 
For things like ASCII, the leading 3 bytes are all 0's. 
What are some tradeoffs between UTF-32 and UTF-8?

ANSWER:
- First of all, it will breaks all code that has been coded in ASCII, 
down to the very basic system that has been written in ASCII since the late 1900s.
Also, the cost to rewrite everything into UTF-32 will be enormous.
- It will enables all string, special characters, other Latin characters, other alphabets and other BUNCH of emojis 
to work FLAWLESSLY and offer better flexibility like how we do it with traditional ASCII.
- BUT, it will takes up a lot of space, A LOT! Some devices with lower memory slots will definitely suffers the most.
Needlessly to mentioned, the moment I opened up Google Chrome, it already takes up a couple of GBs, 
on top of another browser, which takes up another 3 GBs. (MY PC GOT 32 GBs RAM btw)

EXAMPLE: 
- Just like how I've been using traditional ASCII codes for a while now and used to that system.
Now, I'm struggling with coding for UTF-8 because of first time exposure and understanding they works differently.
But it will enable better text outputting, since I can encorporate more stuffs into the string.
- However, the trade off is it will take an enormous amount memory JUST for printing texts,
which goes against memory optimazation standards (until standards of hardware has improved).
- That explains why there is different data types that eats up different bytes to serve for memory optimization.

QUESTION 2: 
UTF-8 has a leading 10 on all the bytes past the first for multi-byte code points. 
This seems wasteful – if the encoding for 3 bytes were instead 1110XXXX XXXXXXXX XXXXXXXX (where X can be any bit), that would fit 20 bits, 
which is over a million code points worth of space, removing the need for a 4-byte encoding. 
What are some tradeoffs or reasons the leading 10 might be useful? 
Can you think of anything that could go wrong with some programs if the encoding didn't include this restriction on multi-byte code points?

ANSWER:
- The standard for ASCII characters only uses up to 7 bits, which is 0XXXXXXX. Therefore the very first bit is set to 0 for all ASCII characters.
- Question is, "what happen if the encoding for 3 bytes were instead 1110XXXX XXXXXXXX XXXXXXXX (where X can be any bit, even 0) [instead of having leading 10xxxxxx?]"
Simply, the continuation bytes will look like an ASCII character, instead of the UTF-8 (as wanted), 
and the compiler will mistaken the bytes as an ASCII, which will have a mix of weird output start bytes and ASCII character. 
- Therefore, since ASCII first bit is always 0, people encoded it as 10xxxxxx then it will easily be detected as a continuation bytes.
==> The design on multi-byte code points makes so much sense with the bytes encoding.