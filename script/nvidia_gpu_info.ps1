$nvidiaOutput = nvidia-smi | findstr "%";
$m_tmp = $nvidiaOutput -split ("\|") -replace '\s{2,}';

$memUsage = $m_tmp[2]
$percent = $m_tmp[3] -split("\%");

Write-Output $percent[0]
Write-Output $memUsage