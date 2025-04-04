$text = Get-Content "message.json" -Raw
$bytes = [System.Text.Encoding]::UTF8.GetBytes($text)
$base64 = [System.Convert]::ToBase64String($bytes)
$base64 | Out-File "message_base64.txt"
