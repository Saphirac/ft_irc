# Introduction
This file is a summary of:
- [RFC 2813](https://www.rfc-editor.org/rfc/pdfrfc/rfc2813.txt.pdf)
- [RFC 2812](https://www.rfc-editor.org/rfc/pdfrfc/rfc2812.txt.pdf)
- [RFC 2811](https://www.rfc-editor.org/rfc/pdfrfc/rfc2811.txt.pdf)
- [RFC 2810](https://www.rfc-editor.org/rfc/pdfrfc/rfc2810.txt.pdf)
- [RFC 1459](https://www.rfc-editor.org/rfc/pdfrfc/rfc1459.txt.pdf)

They are the RFCs that define the IRC protocol.<br>
In this summary, we will only cover the most important parts of the RFCs,<br>
that are relevant to the implementation of our own IRC server.<br>
That means that everything related to the client side of the protocol<br>
and to the server-to-server communication will be voluntarily ignored.

# Server specifications
The server must have a unique name which must be at most 63 characters long.<br>
Server name must be of the following form:<br>
`shortname *( "." shortname )`<br>
where `shortname` is of the following form:<br>
`( letter / digit ) *( letter / digit / "-" )`<br>

The above notation means that the server name must:
1. Start with 1 `shortname` that must:
    1. Start with 1 character that is either a letter or a digit
    2. Contain 0 or more characters that are letters and/or digit and/or dashes (`-`)
2. Contain 0 or more patterns that must:
    1. Start with 1 dot (`.`)
    2. Contain 1 `shortname` that must:
        1. Start with 1 character that is either a letter or a digit
		2. Contain 0 or more characters that are letters and/or digit and/or dashes (`-`)

# User specifications
Every user must have a unique nickname which must be at most 9 characters long.<br>
Nicknames must be of the following form:<br>
`( letter / special ) *8( letter / digit / special / "-" )`<br>
where `special` is one of the following characters:<br>
``"[", "]", "\", "`", "_", "^", "{", "|", "}"``

The above notation means that the nickname must:
1. Start with 1 character that is either a letter or a special character
2. Contain 0 or more up to 8 characters that are letters and/or digits and/or special characters and/or dashes (`-`)

# Message specifications
Messages received by the server are separated by:
- CR (carriage return)(`\r`)
- LF (line feed)(`\n`)

Empty messages are silently ignored.<br>
<br>
For example, receiving the following string:<br>
`"Hello\nWorld\r!\n\n How\n\rare \r\n you \r\r?"`<br>
will result in the following messages being received by the server: <br>
`"Hello", "World", "!", " How", "are ", " you ", "?"`

A message received by the server is composed of 3 parts:
- Prefix
- Command
- Parameters