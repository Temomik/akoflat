<?php
$basePath = "/home/WebApp/";
$json = file_get_contents('php://input');

$data = explode("|", $json);

file_put_contents($basePath . $data[0], $data[1]);
?>