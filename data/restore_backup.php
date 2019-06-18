<?php
$myFile = "config.json";
$myFileBak = ".bak/config.json";
// Will copy config.json to config.json
// overwritting it if necessary
copy($myFileBak,$myFile);

?>
