# Table of contents
- [Table of contents](#table-of-contents)
- [Introduction](#introduction)
- [Server specifications](#server-specifications)
- [User specifications](#user-specifications)
- [Channel specifications](#channel-specifications)
- [Message specifications](#message-specifications)
	- [Prefix](#prefix)
	- [Command](#command)
	- [Parameters](#parameters)
- [Command specifications](#command-specifications)
- [Reply specifications](#reply-specifications)
- [Explained ABNF notations](#explained-abnf-notations)
	- [Short name](#short-name)
	- [Server name / Host](#server-name--host)
	- [User nickname](#user-nickname)
	- [Channel string](#channel-string)
	- [Channel name](#channel-name)
	- [Messages](#messages)
	- [Prefix](#prefix-1)
	- [Command](#command-1)
	- [Parameters](#parameters-1)

# Introduction
This file is a summary of:
- [RFC 2813](https://www.rfc-editor.org/rfc/pdfrfc/rfc2813.txt.pdf) (Server Protocol)
- [RFC 2812](https://www.rfc-editor.org/rfc/pdfrfc/rfc2812.txt.pdf) (Client Protocol)
- [RFC 2811](https://www.rfc-editor.org/rfc/pdfrfc/rfc2811.txt.pdf) (Channel Management)
- [RFC 2810](https://www.rfc-editor.org/rfc/pdfrfc/rfc2810.txt.pdf) (Architecture)
- [RFC 1459](https://www.rfc-editor.org/rfc/pdfrfc/rfc1459.txt.pdf) (IRC Protocol)

They are the RFCs that define the IRC protocol.<br>
In this summary, we will only cover the most important parts of the RFCs,<br>
that are relevant to the implementation of our own IRC server.<br>
That means that everything related to the client side of the protocol<br>
and to the server-to-server communication will be voluntarily ignored.<br>
<br>
Note that we are using the [Augmented Backus–Naur form](https://en.wikipedia.org/wiki/Augmented_Backus%E2%80%93Naur_form)<br>
to represent the grammar rules that apply to the different components of the IRC protocol.<br>
However, we also provided a more human-readable description of the rules for each,<br>
in the [Explained ABNF notations](#explained-abnf-notations) section.<br>

# Server specifications
The server must have a unique name, which must:
- be at most 63 characters long.
- be formated as follows:<br>
	`shortname *( "." shortname )`<br>
	where `shortname` is formatted as follows:<br>
	`( letter / digit ) *( letter / digit / "-" )`

	(click [here](#server-name) for more explanations about the above notation)

# User specifications
For every user, the server must have the following information about them:
- a unique nickname, which must:
	- be at most 9 characters long.<br>
	- be formatted as follows:<br>
		`( letter / special ) *8( letter / digit / special / "-" )`<br>
		where `special` is any of the following characters:<br>
		``[\]^_`{|}``

		(click [here](#user-nickname) for more explanations about the above notation)
- the name of the host that the user is running on
- the username of the user on that host

# Channel specifications
For every channel, the server must have the following information about it:
- a unique name, which must:
	- be at most 50 characters long.<br>
	- be case-insensitive.<br>
	- be formatted as follows:<br>
		`( "#" / "+" / "&" / ( "!" channelid ) ) chanstring [ ":" chanstring ]`<br>
		where:
		- `chanelid` is formatted as follows:<br>
			`5( uppercase / digit )`<br>
		- `chanstring` is formatted as follows:<br>
			`*(%x01-06 / %x08-09 / %x0b-0c / %x0e-1f / %x21-2b / %x2d-39 / %x3b-ff)`

		(click [here](#channel-name) for more explanations about the above notation)
- a list of users that are on that channel
- any other data needed for the channel modes<br>
	(for example, the channel topic, a flag field, a list of the channel operators, etc...)

# Message specifications
Every message received by the server must:
- be at most 512 characters long.
- be formatted as follows:<br>
	`[ ":" prefix space ] command [ params ] crlf`<br>
	where:
	- `prefix` is a pattern, which is formatted as described [here](#prefix),
	- `command` is a pattern, which is formatted as described [here](#command),
	- `params` is a pattern, which is formatted as described [here](#parameters),
	- `crlf` is a carriage return (`\r`) followed by a line feed (`\n`),

	(click [here](#messages) for more explanations about the above notation)

Empty messages are silently ignored.

## Prefix
The purpose of the prefix is to explicitly indicate the true origin of a received message.<br>
However, the prefix is <b style="color:rgb(255 221 0)">optionnal</b>, and thus, if a received message has no prefix,<br>
it is assumed that its true origin is the connection from which it was received.<br>

Every prefix must be formatted as follows:<br>
`servername / ( nickname [ [ "!" user ] "@" host ] )`
where:
- `servername` is formatted as described in the [Server specifications](#server-specifications) section,
- `nickname` is formatted as described in the [User specifications](#user-specifications) section,
- `user` is formatted as follows:<br>
	`*( %x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff )`
- `host` is formatted as `servername`

(click [here](#prefix-1) for more explanations about the above notation)

A prefix may be invalid for two main reasons (excluding the errors of format),<br>
and in both cases, the received messages that contain those invalid prefixes must be discarded:<br>
- when it identifies an unknown client.<br>
- when it identifies a known client that is not the one who sent the message.<br>
In this second case, in addition of discarding the message,<br>
the server should drop the connection with the client who sent it.

## Command
The command part may actually be an IRC command or a numeric reply code.<br>

`1*letter / 3digit`

The command part of the message must be either one of the following:
- a valid IRC command (see [Command specifications](#command-specifications) section)
- a 3 digits number that represents a reply code (see [Reply specifications](#reply-specifications) section)

## Parameters
TODO

# Command specifications

# Reply specifications

# Explained ABNF notations
## Short name
1. Start with 1 character that is either a letter or a digit
1. Contain 0 or more characters that are letters and/or digits and/or dashes (`-`)

## Server name / Host
1. Start with 1 `shortname`, which must be formatted as described [here](#short-name)
1. Contain 0 or more patterns, which must:
	1. Start with 1 dot (`.`)
	2. Contain 1 `shortname`, which must be formatted as described [here](#short-name)

## User nickname
1. Start with 1 character that is either a letter or any of the ``[\]^_`{|}`` characters
1. Contain 0 or more up to 8 characters that are letters and/or digits<br>
and/or any of the ``[\]^_`{|}`` characters and/or dashes (`-`)

## Channel string
1. Start with 0 or more characters that are any octet except:
	- NUL (`\0`)
	- BELL (`\a`)
	- LF (`\n`)
	- CR (`\r`)
	- space (` `)
	- comma (`,`)
	- colon (`:`)

## Channel name
1. Start with 1 character that is either a hash (`#`), a plus (`+`), an ampersand (`&`),<br>
or an exclamation mark (`!`) followed by a 5 characters that are uppercase letters and/or digits
1. Contain a `chanstring`, which must be formatted as described [here](#channel-string)
2. Contain 0 or 1 pattern, which must:
	1. Start with 1 colon (`:`)
	2. Contain a `chanstring`, which must be formatted as described [here](#channel-string)

## Messages
TODO: `[ ":" prefix space ] command [ params ] crlf`

## Prefix
1. Start with either one of the following:
	- `servername`, which must be formatted as described [here](#server-name--host)
	- 1 pattern, which must:
		1. Start with 1 `nickname`, which must be formatted as described [here](#user-nickname)
		2. Contain 0 or 1 pattern, which must:
			1. Start with 0 or 1 pattern, which must:
				1. Start with 1 exclamation mark (`!`)
				2. Contain 1 `user`, which must:
					1. Start with 1 or more characters that are any octet except:
						- NUL (`\0`)
						- LF (`\n`)
						- CR (`\r`)
						- space (` `)
						- at sign (`@`)
			2. Contain 1 at sign (`@`)
			3. Contain 1 `host`, which must be formatted as described [here](#server-name--host)

## Command
TODO

## Parameters
TODO
