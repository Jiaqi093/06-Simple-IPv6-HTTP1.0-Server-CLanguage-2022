# 06-Simple-IPv6-HTTP1.0-Server-CLanguage-2022

My task is to write a simple HTTP Server. There is only a limited range of content that needs to be served, and it only needs to respond to GET requests for staticles. 
Example content that needs to be served is:
- Content File Extension
- HTML -> .html
- JPEG -> .jpg
- CSS  -> .css
JavaScript -> .js

For these extensions, the correct MIME type should be reported (see https://mimetype.io/all- types).
For all other extensions, or if there is no extension like Makefile, I can return a MIME type of application/octet-stream. 

The minimum requirement is that the server implements HTTP 1.0, and the server does not have to support pipelining or persistent connections. My server is able to handle multiple incoming requests by making use of Pthreads (or similar concurrent programming technique) to process and respond to each incoming request.


All paths requested by a client should be treated as relative to the path specied on the command line.
For example, if a request was received with the path /css/style.css and the web directory is /home/comp30023/website, it would be expected to be found
at:

/home/comp30023/website/css/style.css

The server support responding with either a 200 response containing the requested file, or a 404 response if the requested file is not found. 

# Response headers should be valid and must included at a minimum:

- Http Status
- Content-type

# To run server program on VM prompt, type:
./server [protocol number] [port number] [path to web root]
where:
- [protocol number] is 4 for IPv4 or 6 for IPv6
- [port number] is a valid p ort numb er (e.g., 8080), and
- [path to web root] is a valid absolute path (e.g., /home/comp30023/website)
