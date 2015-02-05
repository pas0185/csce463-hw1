5 February 2015
Patrick Sheehan
CSCE 463 HW#1
github.com/pas0185/csce463-hw1

**Part 1**

A simple web client that accepts URLs and crawls them to display basic page statistics. 

To run this program, provide one or more URL strings in the command line arguments, e.g.:
    > hw1.exe http://www.google.com http://aburningpatience.blogspot.com/2012_01_01_archive.html

The three subtasks for this assignment were: 
	1) URL Parsing
	2) DNS Lookup 
	3) Establishing a socket connection, and
	4) Parsing an HTML File

1) The UrlParser takes an input a string (char*) that represents a URL.
	It will decompose properly formatted strings according to the following:
		> scheme://host[:port][/path][?query][#fragment] 

	This class is responsible for building the GET request, and 
	for retrieving the hostname (used in DNS lookup)

2) The WebSocket class accepts a hostname (string or dot-notation) and uses a DNS look up
	to resolve its' IP address

	This class also establishes a connection to a Socket, maintains the descriptor, and 
	uses it to send HTTP requests and receive HTML page responses

3) The HtmlParser uses an existing library to parse the retrieved HTML pages and 
	count the number of hyperlinks on the page


**Part 2**

Downloading multiple URLs from an input file using a single thread. 

Usage:
	> hw1.exe URL-input.txt

Here we've expanded the reach of our webcrawler.
We now check a host's robots.txt file before deciding whether or not to GET the entire page
We also maintain the IP addresses that have already been looked up through a DNS, to reduce redundancies

