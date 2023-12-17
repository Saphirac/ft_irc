# Table of contents
- [Table of contents](#table-of-contents)
- [Introduction](#introduction)
- [Server](#server)
	- [Name](#name)
	- [Version](#version)
- [Users](#users)
	- [Nickname](#nickname)
	- [Hostname](#hostname)
	- [Username](#username)
	- [Modes](#modes)
- [Channels](#channels)
	- [Name](#name-1)
	- [Topic](#topic)
	- [Modes](#modes-1)
- [Messages](#messages)
	- [Prefix](#prefix)
	- [Command](#command)
	- [Parameters](#parameters)
- [Commands](#commands)
- [Numeric replies](#numeric-replies)
	- [Registration](#registration)
	- [Infos](#infos)
	- [Errors](#errors)
- [Explained ABNF notations](#explained-abnf-notations)
	- [Server](#server-1)
		- [Name](#name-2)
		- [Version](#version-1)
	- [User](#user)
		- [Nickname](#nickname-1)
		- [Hostname](#hostname-1)
		- [Username](#username-1)
		- [Modes](#modes-2)
	- [Channel](#channel)
		- [Name](#name-3)
		- [Topic](#topic-1)
		- [Modes](#modes-3)
	- [Message](#message)
		- [Prefix](#prefix-1)
		- [Command](#command-1)
		- [Parameters](#parameters-1)
	- [Replies](#replies)
		- [RPL\_WELCOME](#rpl_welcome)
		- [RPL\_YOURHOST](#rpl_yourhost)
		- [RPL\_CREATED](#rpl_created)
		- [RPL\_MYINFO](#rpl_myinfo)
		- [RPL\_UMODEIS](#rpl_umodeis)
		- [RPL\_YOUREOPER](#rpl_youreoper)
		- [ERR\_NONICKNAMEGIVEN](#err_nonicknamegiven)
		- [ERR\_ERRONEUSNICKNAME](#err_erroneusnickname)
		- [ERR\_NICKNAMEINUSE](#err_nicknameinuse)
		- [ERR\_NEEDMOREPARAMS](#err_needmoreparams)
		- [ERR\_ALREADYREGISTERED](#err_alreadyregistered)
		- [ERR\_PASSWDMISMATCH](#err_passwdmismatch)
		- [ERR\_NOOPERHOST](#err_nooperhost)
		- [ERR\_UMODEUNKNOWNFLAG](#err_umodeunknownflag)
		- [ERR\_USERSDONTMATCH](#err_usersdontmatch)
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
and to the server-to-server communication will be voluntarily ignored.

Note that we are using the [Augmented Backus–Naur form](https://en.wikipedia.org/wiki/Augmented_Backus%E2%80%93Naur_form)<br>
to represent the grammar rules that apply to the different components of the IRC protocol.<br>
However, we also provided a more human-readable description of the rules for each,<br>
in the [Explained ABNF notations](#explained-abnf-notations) section.

# Server
The server must have:
- a [name](#name)
- a [version](#version)

## Name
The server name must:
- be at most 63 characters long.
- be formated as follows:<br>
`shortname *( "." shortname )`

where `shortname` is formatted as follows:<br>
`( letter / digit ) *( letter / digit / "-" )`

(click [here](#name-2) for more explanations about the above notation)

## Version
The server version must be formatted as follows:<br>
`1*digit *( "." 1*digit )`

(click [here](#version-1) for more explanations about the above notation)

# Users
For every user, the server must keep track of the following information about them:
- their [nickname](#nickname), which is the name used by the user on the IRC network
- their [hostname](#hostname), which is the name of the host that the user is running on
- their [username](#username), which is the name of the user on their host
- their [modes](#modes), which are a set of flags that indicate different states about the user

## Nickname
Every nickname must:
- be unique
- be at most 9 characters long.
- be formatted as follows:<br>
	`( letter / special ) *8( letter / digit / special / "-" )`

	where `special` is formatted as follows:<br>
	`%x5b-60 / %x7b-7d`

	(click [here](#nickname-1) for more explanations about the above notation)

## Hostname
Every hostname must be formatted as follows:<br>
`shortname *( "." shortname )`

where `shortname` is formatted as follows:<br>
`( letter / digit ) *( letter / digit / "-" )`

(click [here](#hostname-1) for more explanations about the above notation)

## Username
Every username must be formatted as follows:<br>
`1*( %x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff )`

(click [here](#username-1) for more explanations about the above notation)

## Modes
In messages sent and received by the server, user modes are represented by a single letter.

Here is a list of the user modes that the server will support:
- `a` : away
- `i` : invisible
- `w` : wallops listener
- `B` : bot
- `O` : local operator

Every set of modes related to a user that is received or sent by the server<br>
must be formatted as follows:<br>
`*( "a" / "i" / "w" / "B" / "O" )`

(click [here](#modes-2) for more explanations about the above notation)

# Channels
For every channel, the server must keep track of the following information about it:
- their [name](#name-1)
- their [topic](#topic), which is a short description of the channel
- their [modes](#modes-1), which are a set of flags that indicate different states about the channel<br>
	and/or additional fields that are needed by some modes<br>
	(e.g. a list of channel operators, a list of banned users, the key to join the channel, etc...)
- a __list of users__ that are on that channel

## Name
Every channel name must:
- be at most 50 characters long.<br>
- be case-insensitive.<br>
- be formatted as follows:<br>
	`( "#" / "+" / "&" / ( "!" channelid ) ) chanstring [ ":" chanstring ]`<br>

	where:
	- `channelid` is formatted as follows:<br>
		`5( uppercase / digit )`<br>
	- `chanstring` is formatted as follows:<br>
		`*( %x01-06 / %x08-09 / %x0b-0c / %x0e-1f / %x21-2b / %x2d-39 / %x3b-ff )`

		(click [here](#name-3) for more explanations about the above notation)

## Topic
Every channel topic must be formatted as follows:<br>
`*( %x01-09 / %x0b-0c / %x0e-ff )`

(click [here](#topic-1) for more explanations about the above notation)

## Modes
Channels may have a set of modes, to indicate different states about themselves.<br>
Unlike the user modes, some channel modes may be related to some users,<br>
and not only to the channel itself. Therefore, channels may need to have extra fields<br>
to keep track of the users that are related to these modes.<br>
In messages sent and received by the server, channel modes are represented by a single letter.

Here is a list of the channel modes that the server will support:
- `b` : set/remove ban mask
- `i` : invite-only
- `k` : key needed to join
- `l` : limit on number of users
- `n` : no messages to channel from clients on the outside
- `o` : give/take channel operator privileges
- `t` : topic settable by channel operators only

Every set of modes related to a channel that is received or sent by the server<br>
must be formatted as follows:<br>
`*( "b" / "i" / "k" / "l" / "n" / "o" / "t" )`

(click [here](#modes-3) for more explanations about the above notation)

# Messages
Every message sent and received by the server must:
- be at most 512 characters long.
- be formatted as follows:<br>
	`[ ":" prefix space ] command [ params ] crlf`<br>
	
	where:
	- `prefix` is formatted as described [here](#prefix)
	- `command` is formatted as described [here](#command)
	- `params` is formatted as described [here](#parameters)
	- `crlf` is a carriage return (`\r`) followed by a line feed (`\n`)

	(click [here](#message) for more explanations about the above notation)

Empty messages are silently ignored.

## Prefix
The purpose of the prefix is to explicitly indicate the true origin of a received message.<br>
However, the prefix is <b style="color:rgb(255 221 0)">optionnal</b>,
and thus, if a received message has no prefix,<br>
it is assumed that its true origin is the connection from which it was received.<br>

Every prefix must be formatted as either one of the following:<br>
- `server`
- `nick [ [ "!" user ] "@" host ]`

where:
- `server` is formatted as described [here](#name)
- `nick` is formatted as described [here](#nickname)
- `user` is formatted as descibed [here](#username)
- `host` is formatted as described [here](#hostname)

(click [here](#prefix-1) for more explanations about the above notations)

A prefix may be invalid for 3 different reasons, and in each case,<br>
the received message that contain this invalid prefix must be discarded,<br>
and an error must be sent back to the client from which the message was received.<br>
A prefix is considered invalid when:
- it has a format error
- it identifies an unknown client
- it identifies a known client that is not the one who sent the message<br>
	In this last case, the server should drop the connection with the client who sent it

## Command
The command part of the message must be either one of the following:
- a valid [IRC command](#commands), which must be formatted as follows:<br>
	`1*letter`
- a [numeric reply](#numeric-replies), which must be formatted as follows:<br>
	`3digit`

(click [here](#command-1) for more explanations about the above notations)

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

(click [here](#parameters-1) for more explanations about the above notations)

# Commands

# Numeric replies
Most of the messages that the server receives from a client<br>
generate a reply to that client from the server to indicate<br>
whether the operation succeeded or failed.<br>

Numeric replies are not allowed to originate from a client;<br>
any such messages received by the server must be silently discarded.<br>

Here is a list of the different numeric replies that we will need.

## Registration
- 001: RPL_WELCOME<br>
	`"Welcome to the Internet Relay Network" space nick "!" user "@" host`<br>

	where:
	- `nick` is formatted as described [here](#nickname),
	- `user` is formatted as described [here](#username),
	- `host` is formatted as described [here](#hostname)

	(click [here](#rpl_welcome) for more explanations about the above notation)

- 002: RPL_YOURHOST<br>
	`"Your host is" space server "," space "running version" space version`

	where:
	- `server` is formatted as described [here](#name)
	- `version` is formatted as described [here](#version)

	(click [here](#rpl_yourhost) for more explanations about the above notation)

- 003: RPL_CREATED<br>
	`"This server was created" space date`

	where:
	- `date` is formatted as follows:<br>
		`year "-" month "-" day`
	- `year` is formatted as follows:<br>
		`1*digit`
	- `month` is formatted as follows:<br>
		`%x30 %x31-39 / %x31 %x30-32`
	- `day` is formatted as follows:<br>
		`%x30 %x31-39 / %x31-32 %x30-39 / %x33 %x30-31`

	(click [here](#rpl_created) for more explanations about the above notation)

- 004: RPL_MYINFO<br>
	`server space version space umodes space cmodes`

	where:
	- `server` is formatted as described [here](#name)
	- `version` is formatted as described [here](#version)
	- `umodes` represents the available user modes, and is formatted as described [here](#modes)
	- `cmodes` represents the available channel modes, and is formatted as described [here](#modes-1)

	(click [here](#rpl_myinfo) for more explanations about the above notation)

## Infos
- 221: RPL_UMODEIS<br>
	`nick space ":" [ "+" ] umodes`

	where:
	- `nick` is formatted as described [here](#nickname)
	- `umodes` is formatted as described [here](#modes)

	(click [here](#rpl_umodeis) for more explanations about the above notation)

- 381: RPL_YOUREOPER<br>
	`":You are now an IRC operator"`

	(click [here](#rpl_youreoper) for more explanations about the above notation)

## Errors
- 431: ERR_NONICKNAMEGIVEN<br>
	`":No nickname given"`

	(click [here](#err_nonicknamegiven) for more explanations about the above notation)

- 432: ERR_ERRONEUSNICKNAME<br>
	`nick space ":Erroneous nickname"`

	where `nick` is formatted as described [here](#nickname)

	(click [here](#err_erroneusnickname) for more explanations about the above notation)

- 433: ERR_NICKNAMEINUSE<br>
	`nick space ":Nickname already in use"`

	where `nick` is formatted as described [here](#nickname)

	(click [here](#err_nicknameinuse) for more explanations about the above notation)

- 461: ERR_NEEDMOREPARAMS<br>
	`command space ":Not enough parameters"`

	where `command` is formatted as described [here](#command)

	(click [here](#err_needmoreparams) for more explanations about the above notation)

- 462: ERR_ALREADYREGISTERED<br>
	`":Unauthorized command (already registered)"`

	(click [here](#err_alreadyregistered) for more explanations about the above notation)

- 464: ERR_PASSWDMISMATCH<br>
	`":Password incorrect"`

	(click [here](#err_passwdmismatch) for more explanations about the above notation)

- 491: ERR_NOOPERHOST<br>
	`":No O-lines for your host"`

	(click [here](#err_nooperhost) for more explanations about the above notation)

- 501: ERR_UMODEUNKNOWNFLAG<br>
	`":Unknown MODE flag"`

	(click [here](#err_umodeunknownflag) for more explanations about the above notation)

- 502: ERR_USERSDONTMATCH<br>
	`":Cannot change mode for other users"`

	(click [here](#err_usersdontmatch) for more explanations about the above notation)

# Explained ABNF notations
## Server
### Name
`shortname *( "." shortname )`
>	1. Start with 1 [`shortname`](#shortname)
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 dot (`.`)
>		1. Contain 1 [`shortname`](#shortname)

### Version
`1*digit *( "." 1*digit )`
>	1. Start with 1 or more digits
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 dot (`.`)
>		1. Contain 1 or more digits

## User
### Nickname
`( letter / special ) *8( letter / digit / special / "-" )`
>	1. Start with 1 character that is either a letter or a [`special`](#special)
>	1. Contain 0 or more up to 8 characters that are letters<br>
>	and/or digits and/or a [`special`](#special) and/or dashes (`-`)

### Hostname
`shortname *( "." shortname )`
>	1. Start with 1 [`shortname`](#shortname)
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 dot (`.`)
>		1. Contain 1 [`shortname`](#shortname)

### Username
`1*( %x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff )`
>	1 or more characters that are any octet except:
>	- NUL (`\0`)
>	- CR (`\r`)
>	- LF (`\n`)
>	- space (` `)
>	- at sign (`@`)

### Modes
`*( "a" / "i" / "w" / "B" / "O" )`
>	0 or more characters that are any of the following:
>	- lower A (`a`)
>	- lower I (`i`)
>	- lower W (`w`)
>	- upper B (`B`)
>	- upper O (`O`)

## Channel
### Name
`( "#" / "+" / "&" / ( "!" channelid ) ) chanstring [ ":" chanstring ]`
>	1. Start with 1 character that is either a hash (`#`), a plus (`+`), an ampersand (`&`),<br>
>	or a pattern, which must:
>		1. Start with 1 exclamation mark (`!`)
>		1. Contain 1 [`channelid`](#channelid)
>	1. Contain a [`chanstring`](#chanstring)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 1 colon (`:`)
>		1. Contain a [`chanstring`](#chanstring)

### Topic
`*( %x01-09 / %x0b-0c / %x0e-ff )`
>	0 or more characters that are any octet except:
>	- NUL (`\0`)
>	- CR (`\r`)
>	- LF (`\n`)

### Modes
`*( "b" / "i" / "k" / "l" / "n" / "o" / "t" )`
>	0 or more characters that are any of the following:
>	- lower B (`b`)
>	- lower I (`i`)
>	- lower K (`k`)
>	- lower L (`l`)
>	- lower N (`n`)
>	- lower O (`o`)
>	- lower T (`t`)

## Message
`[ ":" prefix space ] command [ params ] crlf`

>	1. Start with 0 or 1 pattern, which must:
>		1. Start with 1 colon (`:`)
>		1. Contain 1 [`prefix`](#prefix-1)
>		1. Contain 1 space (` `)
>	1. Contain 1 [`command`](#command-1)
>	1. Contain 0 or 1 [`params`](#parameters-1)
>	1. Contain 1 [`crlf`](#crlf)

### Prefix
- `server`
>	1 [`server`](#name-2)
- `nick [ [ "!" user ] "@" host ]`
>	1. Start with 1 [`nick`](#nickname-1)
>	1. Contain 0 or 1 pattern, which must:
>		1. Start with 0 or 1 pattern, which must:
>			1. Start with 1 exclamation mark (`!`)
>			1. Contain 1 [`user`](#username-1)
>		1. Contain 1 at sign (`@`)
>		1. Contain 1 [`host`](#hostname-1)

### Command
- `1*letter`
>	1 or more letters

- `3digit`
>	3 digits

### Parameters
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

## Replies
### RPL_WELCOME
`"Welcome to the Internet Relay Network" space nick "!" user "@" host`
>	1. Start with the string "Welcome to the Internet Relay Network"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`nick`](#nickname-1)
>	1. Contain 1 exclamation mark (`!`)
>	1. Contain 1 [`user`](#username-1)
>	1. Contain 1 at sign (`@`)
>	1. Contain 1 [`host`](#hostname-1)

### RPL_YOURHOST
`"Your host is" space server "," space "running version" space version`
>	1. Start with the string "Your host is"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`server`](#name-2)
>	1. Contain 1 comma (`,`)
>	1. Contain 1 space (` `)
>	1. Contain the string "running version"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`version`](#version-1)

### RPL_CREATED
`"This server was created" space date`
>	1. Start with the string "This server was created"
>	1. Contain 1 space (` `)
>	1. Contain 1 [`date`](#date)

### RPL_MYINFO
`server space version space umodes space cmodes`
>	1. Start with 1 [`server`](#name-2)
>	1. Contain 1 space (` `)
>	1. Contain 1 [`version`](#version-1)
>	1. Contain 1 space (` `)
>	1. Contain 1 [`umodes`](#modes-2)
>	1. Contain 1 space (` `)
>	1. Contain 1 [`cmodes`](#modes-3)

### RPL_UMODEIS
`nick space ":" [ "+" ] umodes`
>	1. Start with 1 [`nick`](#nickname-1)
>	1. Contain 1 space (` `)
>	1. Contain 1 colon (`:`)
>	1. Contain 0 or 1 plus (`+`)
>	1. Contain 1 [`umodes`](#modes-2)

### RPL_YOUREOPER
`":You are now an IRC operator"`
>	Match the string ":You are now an IRC operator"

### ERR_NONICKNAMEGIVEN
`":No nickname given"`
>	Match the string ":No nickname given"

### ERR_ERRONEUSNICKNAME
`nick space ":Erroneous nickname"`
>	1. Start with 1 [`nick`](#nickname-1)
>	1. Contain 1 space (` `)
>	1. Contain the string ":Erroneous nickname"

### ERR_NICKNAMEINUSE
`nick space ":Nickname already in use"`
>	1. Start with 1 [`nick`](#nickname-1)
>	1. Contain 1 space (` `)
>	1. Contain the string ":Nickname already in use"

### ERR_NEEDMOREPARAMS
`command space ":Not enough parameters"`
>	1. Start with 1 [`command`](#command-1)
>	1. Contain 1 space (` `)
>	1. Contain the string ":Not enough parameters"

### ERR_ALREADYREGISTERED
`":Unauthorized command (already registered)"`
>	Match the string ":Unauthorized command (already registered)"

### ERR_PASSWDMISMATCH
`":Password incorrect"`
>	Match the string ":Password incorrect"

### ERR_NOOPERHOST
`":No O-lines for your host"`
>	Match the string ":No O-lines for your host"

### ERR_UMODEUNKNOWNFLAG
`":Unknown MODE flag"`
>	Match the string ":Unknown MODE flag"

### ERR_USERSDONTMATCH
`":Cannot change mode for other users"`
>	Match the string ":Cannot change mode for other users"

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
`1*digit`
>	1 or more digits

### month
`%x30 %x31-39 / %x31 %x30-32`
>	2 digits that are either:
>	- zero (`0`) followed by any digit from one (`1`) to nine (`9`)
>	- one (`1`) followed by any digit from zero (`0`) to two (`2`)

### day
`%x30 %x31-39 / %x31-32 %x30-39 / %x33 %x30-31`
>	2 digits that are either:
>	- zero (`0`) followed by any digit from one (`1`) to nine (`9`)
>	- one (`1`) or two (`2`) followed by any digit from zero (`0`) to nine (`9`)
>	- three (`3`) followed by any digit from zero (`0`) to one (`1`)