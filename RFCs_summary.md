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
- [Numeric reply specifications](#numeric-reply-specifications)
- [Explained ABNF notations](#explained-abnf-notations)
	- [Server name / Host name](#server-name--host-name)
	- [User name](#user-name)
	- [User nickname](#user-nickname)
	- [Channel name](#channel-name)
	- [Message](#message)
	- [Prefix](#prefix-1)
	- [Command](#command-1)
	- [Parameters](#parameters-1)
	- [Version](#version)
	- [Replies](#replies)
		- [RPL\_WELCOME](#rpl_welcome)
		- [RPL\_YOURHOST](#rpl_yourhost)
		- [RPL\_CREATED](#rpl_created)
	- [Miscellaneaous](#miscellaneaous)
		- [shortname](#shortname)
		- [special](#special)
		- [channelid](#channelid)
		- [chanstring](#chanstring)
		- [nospcrlfcl](#nospcrlfcl)
		- [middle](#middle)
		- [trailing](#trailing)
		- [crlf](#crlf)
		- [date](#date)
		- [year](#year)
		- [month](#month)
		- [day](#day)

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

	(click [here](#server-name--host-name) for more explanations about the above notation)

# User specifications
For every user, the server must have the following information about them:
- a unique nickname, which must:
	- be at most 9 characters long.<br>
	- be formatted as follows:<br>
		`( letter / special ) *8( letter / digit / special / "-" )`<br>

		where `special` is formatted as follows:<br>
		`%x5b-60 / %x7b-7d`

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
		- `channelid` is formatted as follows:<br>
			`5( uppercase / digit )`<br>
		- `chanstring` is formatted as follows:<br>
			`*( %x01-06 / %x08-09 / %x0b-0c / %x0e-1f / %x21-2b / %x2d-39 / %x3b-ff )`

		(click [here](#channel-name) for more explanations about the above notation)
- a list of users that are on that channel
- any other data needed for the channel modes<br>
	(for example, the channel topic, a flag field, a list of the channel operators, etc...)

# Message specifications
Every message sent and received by the server must:
- be at most 512 characters long.
- be formatted as follows:<br>
	`[ ":" prefix space ] command [ params ] crlf`<br>
	
	where:
	- `prefix` is a pattern, which is formatted as described [here](#prefix),
	- `command` is a pattern, which is formatted as described [here](#command),
	- `params` is a pattern, which is formatted as described [here](#parameters),
	- `crlf` is a carriage return (`\r`) followed by a line feed (`\n`),

	(click [here](#message) for more explanations about the above notation)

Empty messages are silently ignored.

## Prefix
The purpose of the prefix is to explicitly indicate the true origin of a received message.<br>
However, the prefix is <b style="color:rgb(255 221 0)">optionnal</b>, and thus, if a received message has no prefix,<br>
it is assumed that its true origin is the connection from which it was received.<br>

Every prefix must be formatted as either one of the following:<br>
- `server`
- `nick [ [ "!" user ] "@" host ]`

where:
- `server` is formatted as described [here](#server-specifications),
- `nick` is formatted as described [here](#user-specifications),
- `user` is formatted as follows:<br>
	`1*( %x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff )`
- `host` is formatted as `server`

(click [here](#prefix-1) for more explanations about the above notation)

A prefix may be invalid for two main reasons (excluding the errors of format),<br>
and in both cases, the received messages that contain those invalid prefixes<br>
must be silently discarded:<br>
- when it identifies an unknown client.<br>
- when it identifies a known client that is not the one who sent the message.<br>
In this second case, in addition of discarding the message,<br>
the server should drop the connection with the client who sent it.

## Command
The command part of the message must be either one of the following:
- a valid IRC command (see [Command specifications](#command-specifications) section),<br>
	which must be formatted as follows:<br>
	`1*letter`
- a numeric reply code (see [Numeric reply specifications](#numeric-reply-specifications) section),<br>
	which must be formatted as follows:<br>
	`3digit`

(click [here](#command-1) for more explanations about the above notation)

## Parameters
The parameters part of the message must be formatted as either one of the following:
- `*14( space middle ) [ space ":" trailing ]`
- `14( space middle ) [ space [ ":" ] trailing ]`

where:
- `middle` is formatted as follows:<br>
	`nospcrlfcl *( ":" / nospcrlfcl )`
- `trailing` is formatted as follows:<br>
	`*( ":" / " " / nospcrlfcl )`
- `nospcrlfcl` is formatted as follows:<br>
	`%x01-09 / %x0b-0c / %x0e-1f / %x21-39 / %x3b-ff`

(click [here](#parameters-1) for more explanations about the above notation)

# Command specifications

# Numeric reply specifications
Most of the messages that the server receives from a client<br>
generate a reply to that client from the server to indicate<br>
whether the operation succeeded or failed.<br>

Numeric replies are not allowed to originate from a client;<br>
any such messages received by the server must be silently discarded.<br>

Here is a list of the different numeric replies that we will need:
- `001` : `RPL_WELCOME`<br>
	`"Welcome to the Internet Relay Network" space nick "!" user "@" host`<br>

	(click [here](#rpl_welcome) for more explanations about the above notation)

- `002` : `RPL_YOURHOST`<br>
	`"Your host is" space server "," space "running version" space version`

	(click [here](#rpl_yourhost) for more explanations about the above notation)

- `003` : `RPL_CREATED`<br>
	`"This server was created" space date`

	(click [here](#rpl_created) for more explanations about the above notation)

- TODO: Continue here

# Explained ABNF notations
## Server name / Host name
`shortname *( "." shortname )`
>	1. Start with 1 [`shortname`](#shortname)
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 dot (`.`)
>		1. Contain 1 [`shortname`](#shortname)

## User name
`1*( %x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff )`
>	1 or more characters that are any octet except:
>	- NUL (`\0`)
>	- CR (`\r`)
>	- LF (`\n`)
>	- space (` `)
>	- at sign (`@`)

## User nickname
`( letter / special ) *8( letter / digit / special / "-" )`
>	1. Start with 1 character that is either a letter or a [`special`](#special)
>	1. Contain 0 or more up to 8 characters that are letters<br>
>	and/or digits and/or a [`special`](#special) and/or dashes (`-`)

## Channel name
`( "#" / "+" / "&" / ( "!" channelid ) ) chanstring [ ":" chanstring ]`
>	1. Start with 1 character that is either a hash (`#`), a plus (`+`), an ampersand (`&`),<br>
>	or a pattern, which must:
>		1. Start with 1 exclamation mark (`!`)
>		1. Contain 1 [`channelid`](#channelid)
>	1. Contain a [`chanstring`](#chanstring)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 1 colon (`:`)
>		1. Contain a [`chanstring`](#chanstring)

## Message
`[ ":" prefix space ] command [ params ] crlf`

>	1. Start with 0 or 1 pattern, which must:
>		1. Start with 1 colon (`:`)
>		1. Contain 1 [`prefix`](#prefix)
>		1. Contain 1 space (` `)
>	1. Contain 1 [`command`](#command)
>	1. Contain 0 or 1 [`params`](#parameters)
>	1. Contain 1 [`crlf`](#crlf)

## Prefix
- `server`
>	1 [`server`](#server-name--host-name)
- `nick [ [ "!" user ] "@" host ]`
>	1. Start with 1 [`nick`](#user-nickname)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 0 or 1 pattern, which must:
>			1. Start with 1 exclamation mark (`!`)
>			1. Contain 1 [`user`](#user-name)
>		1. Contain 1 at sign (`@`)
>		1. Contain 1 [`host`](#server-name--host-name)

## Command
- `1*letter`
>	1 or more letters

- `3digit`
>	3 digits

## Parameters
- `*14( space middle ) [ space ":" trailing ]`
>	1. Start with 0 or more up to 14 patterns, which must:
>		1. Start with 1 space (` `)
>		1. Contain 1 [`middle`](#middle)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 1 space (` `)
>		1. Contain 1 colon (`:`)
>		1. Contain 1 [`trailing`](#trailing)

- `14( space middle ) [ space [ ":" ] trailing ]`
>	1. Start with 14 patterns, which must:
>		1. Start with 1 space (` `)
>		1. Contain 1 [`middle`](#middle)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 1 space (` `)
>		1. Contain 0 or 1 colon (`:`)
>		1. Contain 1 [`trailing`](#trailing)

## Version
`1*digit *( "." 1*digit )`
>	1. Start with 1 or more digits
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 dot (`.`)
>		1. Contain 1 or more digits

## Replies
### RPL_WELCOME
`"Welcome to the Internet Relay Network" space nick "!" user "@" host`
>	1. Start with the string "Welcome to the Internet Relay Network"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`nick`](#user-nickname)
>	1. Contain 1 exclamation mark (`!`)
>	1. Contain 1 [`user`](#user-name)
>	1. Contain 1 at sign (`@`)
>	1. Contain 1 [`host`](#server-name--host-name)

### RPL_YOURHOST
`"Your host is" space server "," space "running version" space version`
>	1. Start with the string "Your host is"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`server`](#server-name--host-name)
>	1. Contain 1 comma (`,`)
>	1. Contain 1 space (` `)
>	1. Contain the string "running version"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`version`](#version)

### RPL_CREATED
`"This server was created" space date`
>	1. Start with the string "This server was created"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`date`](#date)

## Miscellaneaous
### shortname
`( letter / digit ) *( letter / digit / "-" )`
>	1. Start with 1 character that is either a letter or a digit
>	1. Contain 0 or more characters that are letters and/or digits and/or dashes (`-`)

### special
`%x5b-60 / %x7b-7d`
>	1 character that is any of the following:
>	- opening bracket (`[`)
>	- closing bracket (`]`)
>	- opening brace (`{`)
>	- closing brace (`}`)
>	- underscore (`_`)
>	- backslash (`\`)
>	- caret (`^`)
>	- pipe (`|`)
>	- tilde (`` ` ``)

### channelid
`5( uppercase / digit )`
>	5 characters that are either uppercase letters and/or digits

### chanstring
`*( %x01-06 / %x08-09 / %x0b-0c / %x0e-1f / %x21-2b / %x2d-39 / %x3b-ff )`
>	0 or more characters that are any octet except:
>	- NUL (`\0`)
>	- BELL (`\a`)
>	- CR (`\r`)
>	- LF (`\n`)
>	- space (` `)
>	- comma (`,`)
>	- colon (`:`)

### nospcrlfcl
`%x01-09 / %x0b-0c / %x0e-1f / %x21-39 / %x3b-ff`
>	1 character that is any octet except:
>	- NUL (`\0`)
>	- space (` `)
>	- CR (`\r`)
>	- LF (`\n`)
>	- colon (`:`)

### middle
`nospcrlfcl *( ":" / nospcrlfcl )`
>	1. Start with 1 [`nospcrlfcl`](#nospcrlfcl)
>	1. Contain 0 or more colon (`:`) and/or [`nospcrlfcl`](#nospcrlfcl)

### trailing
`*( ":" / " " / nospcrlfcl )`
>	0 or more colon (`:`) and/or space (` `) and/or [`nospcrlfcl`](#nospcrlfcl)

### crlf
`%x0d.0a`
>	1 carriage return (`\r`) followed by 1 line feed (`\n`)

### date
`year "-" month "-" day`
>	1. Start with 1 [`year`](#year)
>	1. Contain 1 dash (`-`)
>	1. Contain 1 [`month`](#month)
>	1. Contain 1 dash (`-`)
>	1. Contain 1 [`day`](#day)

### year
TODO

### month
TODO

### day
TODO