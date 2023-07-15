<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['name'];
    $message = $_POST['message'];
?>
<!DOCTYPE html>
<html>
<head>
    <title>Submitted Form Data</title>
</head>
<body>
    <h1>Submitted Form Data</h1>
    <p><strong>Name:</strong> <?php echo htmlspecialchars($name); ?></p>
    <p><strong>Message:</strong> <?php echo htmlspecialchars($message); ?></p>
</body>
</html>
<?php
}
?>
