<?php
// Path to the static HTML file
$htmlFilePath = "simple.html";

// Check if the HTML file exists
if (file_exists($htmlFilePath)) {
    // Set the content-type header to HTML
    header("Content-type: text/html");

    // Read and output the content of the HTML file
    readfile($htmlFilePath);
} else {
    // Output an error message if the HTML file doesn't exist
    echo "Error: Static HTML file not found";
}
?>
