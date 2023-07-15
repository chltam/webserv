
# Import CGI and CGIT module
import cgi, cgitb			

# to create instance of FieldStorage
# class which we can use to work
# with the submitted form data
form = cgi.FieldStorage()	


print("Content-type:text/html\n")
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Python CGI example</title>")
print("</head>")
print("<body>")
print("<h2>Python CGI called</h2>")
print("</body>")
print("</html>")

