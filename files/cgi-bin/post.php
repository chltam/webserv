<?php
// Read the request body from php://input
$requestBody = file_get_contents('php://input');

// Parse the query string format data
$data = [];
parse_str($requestBody, $data);

// Extract the name and message from the data
$name = $data['name'] ?? '';
$message = $data['message'] ?? '';

// Process the data as needed
// ...

// Generate the HTML response
$html = <<<HTML
<!DOCTYPE html>
<html>
<head>
    <title>Form Submission</title>
</head>
<body>
    <h1>Form Submission</h1>
    <p>Name: $name</p>
    <p>Message: $message</p>
</body>
</html>
HTML;

// Set the Content-Type header to indicate HTML response
header('Content-Type: text/html');

// Output the HTML response
echo $html;
?>
