
# Import CGI and CGIT module
import cgi, cgitb			

# to create instance of FieldStorage
# class which we can use to work
# with the submitted form data
form = cgi.FieldStorage()	
your_name = "test"

# to get the data from fields
company_name = "4242"

print ("Content-type:text/html\n")
print ("<html>")
print ("<head>")
print ("<title>First CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello, %s is working in %s</h2>"
	% (your_name, company_name))

print ("</body>")
print ("</html>")
