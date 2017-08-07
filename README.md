# MP3-tag-reader-editor
A tag reader (or tagger) is a piece of software that supports reading tag’s content and editing tag’s content of mp3 file.

ALGORITHM : Tag Reader

Step 1: Start
Step 2: Read mp3 file name
Step 3: Open file
Step 4: Run loop upto EOF. Find some important tags one by one for read. Get tag content size store it (4 bytes).Ignore flag (2 bytes). Get tag content upto that size and store it 
Step 5 : Print all the tag contents 
Step 6: Stop

ALGORITHM : Tag Editor

Step 1: Start
Step 2: Read mp3 file name, edit option
Step 3: Open file
Step 4: Run loop upto EOF. Find tag to read/edit 
Step 5: Get tag content size and store it (4 bytes) 
Step 6: Read new content/art file of tag upto tag size
Step 7: Write the content to mp3 file
Step 8: Stop

Usage : gcc read.c edit.c main.c
        ./a.out
