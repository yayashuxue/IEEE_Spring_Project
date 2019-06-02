<?php
error_reporting(0); // Disable all errors.
$var1 = $_POST["washerMode"];
$var2 = $_POST["dryerMode"];
$var3 = 1;
$Write="<p>WasherMode: " . $var1 . "</p>"."<p>DryerMode: " . $var2 . "</p>";

$filestatus = file_put_contents('laundryMonitor.html', $Write);
?>

<html>
<body>

Welcome <?php echo $_POST["washerMode"]; ?><br>
Your email address is: <?php echo $_POST["dryerMode"]; ?>

</body>
</html>
