# Webserv

Our own implementation of an Nginx style server in C++, co-worked with [cbadura](https://github.com/cbadura) and [MrMOlten](https://github.com/MrM0lten). We used the socket API from C to listen on multiple ports to receive requests from a client. We also used poll() from C to handle multiplexing. Requests are parsed and responses are sent back accordingly, building the HTTP header and body. Depending on request and endpoint, this process may be going through a CGI – both PHP and Python CGIs are working.

You can make GET, POST, and DELETE requests. To change the default config, just edit the file (e.g. set auto index on and off, set the body size for POST requests, etc.)

## Installation

```bash
cd webserv
make
```

## Usage

```bash
./webserv [ config file if you have one, without argument the default config will be loaded ]
```

Use curl or your web browser with localhost:PORT (default is 8080) to see the index page. Check out the /files directory to see what we prepared -> e.g., localhost:8080/upload.html lets you upload files. You can delete them or download them again.
