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
and to the server-to-server communication will be voluntarily ignored.<br>
<br>
Note that we are using the [Augmented Backus–Naur form](https://en.wikipedia.org/wiki/Augmented_Backus%E2%80%93Naur_form)<br>
to represent the grammar rules that apply to the different components of the IRC protocol.<br>
However, we also provided a more human-readable description of the rules for each,<br>
in the [explained ABNF notations](#explained-abnf-notations).<br>

# Server specifications
The server must have a unique name, which must:
- be at most 63 characters long.
- be of the following form:<br>
  `shortname *( "." shortname )`<br>
  where `shortname` is of the following form:<br>
  `( letter / digit ) *( letter / digit / "-" )`<br>
  (click [here](#server-name) for more explanation about the above notation)

# User specifications
For every user, the server must have the following information about them:
- a unique nickname, which must:
  - be at most 9 characters long.<br>
  - be of the following form:<br>
   	`( letter / special ) *8( letter / digit / special / "-" )`<br>
   	where `special` is one of the following characters:<br>
    ``[]\`_^{|}``<br>
    (click [here](#user-nickname) for more explanation about the above notation)
- the name of the host that the user is running on
- the username of the user on that host

# Channel specifications
For every channel, the server must have the following information about it:
- a unique name, which must:
  - be at most 50 characters long.<br>
  - be case-insensitive.<br>
  - be of the following form:<br>
    `( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]`<br>
    where:
    - `chanelid` is of the following form:<br>
      `5( uppercase / digit )`<br>
    - `chanstring` is of the following form:<br>
      `*(%x01-06 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B / %x2D-39 / %x3B-FF)`<br>
    (click [here](#channel-name) for more explanation about the above notation)
- a list of users that are on that channel
- any other data needed for the channel modes<br>
  (for example, the channel topic, a flag field, a list of the channel operators, etc...)

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

# Explained ABNF notations

## Server name
1. Start with 1 `shortname`, which must:
   1. Start with 1 character that is either a letter or a digit
   2. Contain 0 or more characters that are letters and/or digit and/or dashes (`-`)
2. Contain 0 or more patterns, which must:
   1. Start with 1 dot (`.`)
   2. Contain 1 `shortname`, which must:
      1. Start with 1 character that is either a letter or a digit
      2. Contain 0 or more characters that are letters and/or digit and/or dashes (`-`)

## User nickname
1. Start with 1 character that is either a letter or any of the ``[]\`_^{|}`` characters
2. Contain 0 or more up to 8 characters that are letters and/or digits<br>
   and/or any of the ``[]\`_^{|}`` characters and/or dashes (`-`)

## Channel name
1. Start with 1 character that is either a hash (`#`), a plus (`+`),<br>
   an exclamation mark (`!`) followed by a 5 characters that are either uppercase letters or digits,<br>
   or an ampersand (`&`)
2. Contain 0 or more characters that are any octet except:
   - NUL (`\x00`)
   - BELL (`\x07`)
   - LF (`\x0A`)
   - CR (`\x0D`)
   - space (`\x20`)
   - comma (`\x2C`)
   - colon (`\x3A`)
3. Contain 0 or 1 pattern, which must:
   1. Start with 1 colon (`:`)
   2. Contain 0 or more characters that are any octet except:
      - NUL (`\x00`)
      - BELL (`\x07`)
      - LF (`\x0A`)
      - CR (`\x0D`)
      - space (`\x20`)
      - comma (`\x2C`)
      - colon (`\x3A`)
