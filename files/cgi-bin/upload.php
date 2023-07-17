<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_FILES['file'])) {
        $file = $_FILES['file'];

        // Handle the uploaded file here
        // Example: move the file to a specific directory
        $targetDirectory = 'upload';
        $targetFile = $targetDirectory . $file['name'];

        if (move_uploaded_file($file['tmp_name'], $targetFile)) {
            echo 'File uploaded successfully.';
        } else {
            echo 'Failed to upload file.';
        }
    } else {
        echo 'No file uploaded.';
    }
}
?>
