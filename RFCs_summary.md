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
	- [Mask](#mask)
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
- [Miscellaneaous ABNF components](#miscellaneaous-abnf-components)

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
However, we also provided a more human-readable description for each rule.

# Server
The server must have:
- a [name](#name)
- a [version](#version)

## Name
The server name must:
- be at most 63 octets long.
- be formated as follows:<br>
	`shortname *( %x2e shortname )`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with 1 [shortname](#shortname)
	>	1. Contain 0 or more patterns, which must:
	>		1. Start with 1 octet that has the value 0x2e (`.`)
	>		1. Contain 1 [shortname](#shortname)
	</details>
	<!-- #endregion -->

## Version
The server version must be formatted as follows:<br>
`1*digit *( %x2e 1*digit )`
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1. Start with 1 or more [digits](#digit)
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 octet that has the value 0x2e (`.`)
>		1. Contain 1 or more [digits](#digit)
</details>
<!-- #endregion -->
<br>

# Users
For every user, the server must keep track of the following information about them:
- their [nickname](#nickname), which is the name used by the user on the IRC network
- their [hostname](#hostname), which is the name of the host that the user is running on
- their [username](#username), which is the name of the user on their host
- their [modes](#modes), which are a set of flags that indicate different states about the user

## Nickname
Every nickname must:
- be unique
- be at most 9 octets long.
- be formatted as follows:<br>
	`( letter / special ) *8( letter / digit / special / %x2d )`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with either 1 [letter](#letter) or 1 [special](#special)
	>	1. Contain 0 or more up to 8 [letters](#letter) and/or [digits](#digit)<br>
	>	and/or [specials](#special) and/or octets that have the value 0x2d (`-`)
	</details>
	<!-- #endregion -->

## Hostname
Every hostname must be formatted as follows:<br>
`shortname *( %x2e shortname )`
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1. Start with 1 [shortname](#shortname)
>	1. Contain 0 or more patterns, which must:
>		1. Start with 1 octet that has the value 0x2e (`.`)
>		1. Contain 1 [shortname](#shortname)
</details>
<!-- #endregion -->

## Username
Every username must be formatted as follows:<br>
`1*nolfcrspat`
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1 or more [nolfcrspat](#nolfcrspat)
</details>
<!-- #endregion -->

## Modes
In messages sent and received by the server, user modes are represented by a single letter.

Here is a list of the user modes that the server will support:
- `B` : bot
- `O` : local operator
- `a` : away
- `i` : invisible
- `w` : wallops listener

Every single mode related to a user that is received or sent by the server<br>
must be formatted as follows:<br>
__umode__ = `%x42 / %x4f / %x61 / %x69 / %x77`<br id="umode">
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1 octet that has any value among the following:
>	- 0x42 (`B`)
>	- 0x4f (`O`)
>	- 0x61 (`a`)
>	- 0x69 (`i`)
>	- 0x77 (`w`)
</details>
<!-- #endregion -->
<br>

Every set of modes related to a user that is received or sent by the server<br>
must be formatted as follows:<br>
__umodeset__ = `*umode`<br id="umodeset">
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	0 or more [umodes](#umode)
</details>
<!-- #endregion -->
<br>

## Mask
A mask is a way to identify a user without any possible ambiguity.<br>
Every user mask that is received or sent by the server must be formatted as follows:<br>
__umask__ = `nick %x21 user %x40 host`<br id="umask">
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1. Start with 1 [nick](#nickname)
>	1. Contain 1 octet that has the value 0x21 (`!`)
>	1. Contain 1 [user](#username)
>	1. Contain 1 octet that has the value 0x40 (`@`)
>	1. Contain 1 [host](#hostname)
</details>
<!-- #endregion -->

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
- be at most 50 octets long.<br>
- be case-insensitive.<br>
- be formatted as follows:<br>
	`( %x23 / %x26 / %x2b / ( %x21 channelid ) ) chanstring [ %x3a chanstring ]`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with either 1 of the following:
	>		- 1 octet that has any value among the following:
	>			- 0x23 (`#`)
	>			- 0x26 (`&`)
	>			- 0x2b (`+`)
	>		- 1 pattern, which must:
	>			1. Start with 1 octet that has the value 0x21 (`!`)
	>			1. Contain 1 [channelid](#channelid)
	>	1. Contain 1 [chanstring](#chanstring)
	>	1. Contain 0 or 1 pattern, which must:
	>		1. Start with 1 octet that has the value 0x3a (`:`)
	>		1. Contain 1 [chanstring](#chanstring)
	<!-- #endregion -->

## Topic
Every channel topic must be formatted as follows:<br>
`*nolfcr`
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	0 or more [nolfcr](#nolfcr)
</details>
<!-- #endregion -->

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

Every single mode related to a channel that is received or sent by the server<br>
must be formatted as follows:<br>
__cmode__ = `%x62 / %x69 / %x6b / %x6c / %x6e / %x6f / %x74`<br id="cmode">
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	1 octet that has any value among the following:
>	- 0x62 (`b`)
>	- 0x69 (`i`)
>	- 0x6b (`k`)
>	- 0x6c (`l`)
>	- 0x6e (`n`)
>	- 0x6f (`o`)
>	- 0x74 (`t`)
</details>
<!-- #endregion -->
<br>

Every set of modes related to a channel that is received or sent by the server<br>
must be formatted as follows:<br>
__cmodeset__ = `*cmode`<br id="cmodeset">
<!-- #region: Explained ABNF notation -->
<details>
<summary>Explanations about the above ABNF notation</summary>

>	0 or more [cmode](#cmode)
</details>
<!-- #endregion -->
<br>

# Messages
Every message sent and received by the server must:
- be at most 512 octets long.
- be formatted as follows:<br>
	`[ %x3a prefix space ] command [ params ] crlf`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with 0 or 1 pattern, which must:
	>		1. Start with 1 octet that has the value 0x3a (`:`)
	>		1. Contain 1 [prefix](#prefix)
	>		1. Contain 1 [space](#space)
	>	1. Contain 1 [command](#command)
	>	1. Contain 0 or 1 [params](#parameters)
	>	1. Contain 1 [crlf](#crlf)
	</details>
	<!-- #endregion -->

Empty messages are silently ignored.

## Prefix
The purpose of the prefix is to explicitly indicate the true origin of a received message.<br>
However, the prefix is <b style="color:rgb(255 221 0)">optionnal</b>,
and thus, if a received message has no prefix,<br>
it is assumed that its true origin is the connection from which it was received.<br>

Every prefix must be formatted as either one of the following:<br>
- `server`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>
	
	>	1 [server](#name)
	</details>
	<!-- #endregion -->
	<br>
- `nick [ [ %x21 user ] %x40 host ]`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with 1 [nick](#nickname)
	>	1. Contain 0 or 1 pattern, which must:
	>		1. Start with 0 or 1 pattern, which must:
	>			1. Start with 1 octet that has the value 0x21 (`!`)
	>			1. Contain 1 [user](#username)
	>		1. Contain 1 octet that has the value 0x40 (`@`)
	>		1. Contain 1 [host](#hostname)

	</details>
	<!-- #endregion -->
	<br>
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
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 or more [letters](#letter)
	</details>
	<!-- #endregion -->
	<br>
- a [numeric reply](#numeric-replies), which must be formatted as follows:<br>
	`3digit`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	3 [digits](#digit)
	</details>
	<!-- #endregion -->

## Parameters
The parameters part of the message must be formatted as either one of the following:
- `*13( space middle ) [ space %x3a trailing ]`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with 0 or more up to 13 patterns, which must:
	>		1. Start with 1 [space](#space)
	>		1. Contain 1 [middle](#middle)
	>	1. Contain 0 or 1 pattern, which must:
	>		1. Start with 1 [space](#space)
	>		1. Contain 1 octet that has the value 0x3a (`:`)
	>		1. Contain 1 [trailing](#trailing)
	</details>
	<!-- #endregion -->
	<br>
- `14( space middle ) [ space [ %x3a ] trailing ]`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above ABNF notation</summary>

	>	1. Start with 14 patterns, which must:
	>		1. Start with 1 [space](#space)
	>		1. Contain 1 [middle](#middle)
	>	1. Contain 0 or 1 pattern, which must:
	>		1. Start with 1 [space](#space)
	>		1. Contain 0 or 1 octet that has the value 0x3a (`:`)
	>		1. Contain 1 [trailing](#trailing)
	</details>
	<!-- #endregion -->
	<br>

# Commands

# Numeric replies
Most of the messages that the server receives from a client<br>
generate a reply to that client from the server to indicate<br>
whether the operation succeeded or failed.<br>

Numeric replies are not allowed to originate from a client;<br>
any such messages received by the server must be silently discarded.<br>

Here is a list of the different numeric replies that we will need.

## Registration
- 001: RPL_WELCOME<br id="rpl_welcome">
	`"Welcome to the Internet Relay Network " nick %x21 user %x40 host`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with the string "Welcome to the Internet Relay Network "
	>	1. Contain 1 [nick](#nickname)
	>	1. Contain 1 octet that has the value 0x21 (`!`)
	>	1. Contain 1 [user](#username)
	>	1. Contain 1 octet that has the value 0x40 (`@`)
	>	1. Contain 1 [host](#hostname)
	</details>
	<!-- #endregion -->
	<br>
- 002: RPL_YOURHOST<br id="rpl_yourhost">
	`"Your host is " server ", running version " version`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notaion</summary>

	>	1. Start with the string "Your host is "
	>	1. Contain 1 [server](#name)
	>	1. Contain the string ", running version "
	>	1. Contain 1 [version](#version)
	</details>
	<!-- #endregion -->
	<br>
- 003: RPL_CREATED<br id="rpl_created">
	`"This server was created " date`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notaion</summary>

	>	1. Start with the string "This server was created "
	>	1. Contain 1 [date](#date)
	</details>
	<!-- #endregion -->
	<br>
- 004: RPL_MYINFO<br id="rpl_myinfo">
	`server space version space umodeset space cmodeset`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [server](#name)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [version](#version)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [umodeset](#umodeset)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [cmodeset](#cmodeset)
	</details>
	<!-- #endregion -->

## Infos
- 221: RPL_UMODEIS<br id="rpl_umodeis">
	`nick " :" [ %x2b ] umodeset`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [nick](#nickname)
	>	1. Contain the string " :"
	>	1. Contain 0 or 1 octet that has the value 0x2b (`+`)
	>	1. Contain 1 [umodes](#umodeset)
	</details>
	<!-- #endregion -->
	<br>
- 324: RPL_CHANNELMODEIS<br id="rpl_channelmodeis">
	`channel space mode space params`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [mode](#modes-1)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [params](#parameters)
	</details>
	<!-- #endregion -->
	<br>
- 331: RPL_NOTOPIC<br id="rpl_notopic">
- `channel " :No topic is set"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :No topic is set"
	</details>
	<!-- #endregion -->
	<br>
- 332: RPL_TOPIC<br id="rpl_topic">
	`channel " :" topic`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :"
	>	1. Contain 1 [topic](#topic)
	</details>
	<!-- #endregion -->
	<br>
- 346: RPL_INVITELIST<br id="rpl_invitelist">
	`channel space invitemask`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [invitemask](#umask)
	</details>
	<!-- #endregion -->
	<br>
- 347: RPL_ENDOFINVITELIST<br id="rpl_endofinvitelist">
	`channel " :End of channel invite list"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :End of channel invite list"
	</details>
	<!-- #endregion -->
	<br>
- 367: RPL_BANLIST<br id="rpl_banlist">
	`channel space banmask`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain 1 [space](#space)
	>	1. Contain 1 [banmask](#umask)
	</details>
	<!-- #endregion -->
	<br>
- 368: RPL_ENDOFBANLIST<br id="rpl_endofbanlist">
	`channel " :End of channel ban list"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :End of channel ban list"
	</details>
	<!-- #endregion -->
	<br>
- 381: RPL_YOUREOPER<br id="rpl_youreoper">
	`":You are now an IRC operator"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":You are now an IRC operator"
	</details>
	<!-- #endregion -->

## Errors
- 403: ERR_NOSUCHCHANNEL<br id="err_nosuchchannel">
	`channel " :No such channel"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :No such channel"
	</details>
	<!-- #endregion -->
	<br>
- 405: ERR_TOOMANYCHANNELS<br id="err_toomanychannels">
	`channel " :You have joined too many channels"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :You have joined too many channels"
	</details>
	<!-- #endregion -->
	<br>
- 431: ERR_NONICKNAMEGIVEN<br id="err_nonicknamegiven">
	`":No nickname given"`
	<!-- #region: Eplained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":No nickname given"
	</details>
	<!-- #endregion -->
	<br>
- 432: ERR_ERRONEUSNICKNAME<br id="err_erroneusnickname">
	`nick " :Erroneous nickname"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [nick](#nickname)
	>	1. Contain the string " :Erroneous nickname"
	</details>
	<!-- #endregion -->
	<br>
- 433: ERR_NICKNAMEINUSE<br id="err_nicknameinuse">
	`nick " :Nickname already in use"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [nick](#nickname)
	>	1. Contain the string " :Nickname already in use"
	</details>
	<!-- #endregion -->
	<br>
- 441: ERR_USERNOTINCHANNEL<br id="err_usernotinchannel">
	`nick space channel " :They aren't on that channel"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [nick](#nickname)
	> 1. Contain 1 [space](#space)
	> 1. Contain 1 [channel](#name-1)
	> 1. Contain the string " :They aren't on that channel"
	</details>
	<!-- #endregion -->
	<br>
- 442: ERR_NOTONCHANNEL<br id="err_notonchannel">
	`channel " :You're not on that channel"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :You're not on that channel"
	</details>
	<!-- #endregion -->
	<br>
- 461: ERR_NEEDMOREPARAMS<br id="err_needmoreparams">
	`command " :Not enough parameters"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [command](#command)
	>	1. Contain the string " :Not enough parameters"
	</details>
	<!-- #endregion -->
	<br>
- 462: ERR_ALREADYREGISTERED<br id="err_alreadyregistered">
	`":Unauthorized command (already registered)"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":Unauthorized command (already registered)"
	</details>
	<!-- #endregion -->
	<br>
- 464: ERR_PASSWDMISMATCH<br id="err_passwdmismatch">
	`":Password incorrect"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":Password incorrect"
	</details>
	<!-- #endregion -->
	<br>
- 467: ERR_KEYSET<br id="err_keyset">
	`channel " :Channel key already set"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :Channel key already set"
	</details>
	<!-- #endregion -->
	<br>
- 471: ERR_CHANNELISFULL<br id="err_channelisfull">
	`channel " :Cannot join channel (+l)"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [channel](#name-1)
	>	1. Contain the string " :Cannot join channel (+l)"
	</details>
	<!-- #endregion -->
	<br>
- 472: ERR_UNKNOWNMODE<br id="err_unknownmode">
	`cmode " :is unknown mode char to me for " channel`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [cmode](#cmode)
	> 1. Contain the string " :is unknown mode char to me for "
	> 1. Contain 1 [channel](#name-1)
	</details>
	<!-- #endregion -->
	<br>
- 473: ERR_INVITEONLYCHAN<br id="err_inviteonlychan">
	`channel " :Cannot join channel (+i)"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :Cannot join channel (+i)"
	</details>
	<!-- #endregion -->
	<br>
- 474: ERR_BANNEDFROMCHAN<br id="err_bannedfromchan">
	`channel " :Cannot join channel (+b)"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :Cannot join channel (+b)"
	</details>
	<!-- #endregion -->
	<br>
- 475: ERR_BADCHANNELKEY<br id="err_badchannelkey">
	`channel " :Cannot join channel (+k)"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>
	
	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :Cannot join channel (+k)"
	</details>
	<!-- #endregion -->
	<br>
- 482: ERR_CHANOPRIVSNEEDED<br id="err_chanoprivsneeded">
	`channel " :You're not channel operator"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>
	
	> 1. Start with 1 [channel](#name-1)
	> 1. Contain the string " :You're not channel operator"
	</details>
	<!-- #endregion -->
	<br>
- 491: ERR_NOOPERHOST<br id="err_nooperhost">
	`":No O-lines for your host"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":No O-lines for your host"
	</details>
	<!-- #endregion -->
	<br>
- 501: ERR_UMODEUNKNOWNFLAG<br id="err_umodeunknownflag">
	`":Unknown MODE flag"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":Unknown MODE flag"
	</details>
	<!-- #endregion -->
	<br>
- 502: ERR_USERSDONTMATCH<br id="err_userdontmatch">
	`":Cannot change mode for other users"`
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> Match the string ":Cannot change mode for other users"
	</details>
	<!-- #endregion -->
	<br>
# Miscellaneaous ABNF components
- space = `%x20`<br id="space">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has the value 0x20 (` `)
	</details>
	<!-- #endregion -->
	<br>
- crlf = `%x0d.0a`<br id="crlf">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has the values 0x0d (`\r`)
	>	followed by 1 octet that has the value 0x0a (`\n`)
	</details>
	<!-- #endregion -->
	<br>
- lowercase = `%x61-7a`<br id="lowercase">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value between 0x61 (`a`) and 0x7a (`z`) both included 
	</details>
	<!-- #endregion -->
	<br>
- uppercase = `%x41-5a`<br id="uppercase">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value between 0x41 (`A`) and 0x5a (`Z`) both included
	</details>
	<!-- #endregion -->
	<br>
- letter = `lowercase / uppercase`<br id="letter">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	> 1 octet that is either a [lowercase](#lowercase) or an [uppercase](#uppercase)
	</details>
	<!-- #endregion -->
	<br>
- digit = `%x30-39`<br id="digit">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value between 0x30 (`0`) and 0x39 (`9`) both included
	</details>
	<!-- #endregion -->
	<br>
- special = `%x5b-60 / %x7b-7d`<br id="special">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value among the following:
	>	- 0x5b (`[)	>	- 0x5c (`\`)
	>	- 0x5d (`]`)
	>	- 0x5e (`^`)
	>	- 0x5f (`_`)
	>	- 0x60 (`` ` ``)
	>	- 0x7b (`{`)
	>	- 0x7c (`|`)
	>	- 0x7d (`}`)
	</details>
	<!-- #endregion -->
	<br>
- nolfcr = `%x01-09 / %x0b-0c / %x0e-ff`<br id="nolfcr">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value except:
	>	- 0x00 (`\0`)
	>	- 0x0a (`\n`)
	>	- 0x0d (`\r`)
	</details>
	<!-- #endregion -->
	<br>
- nolfcrsp = `%x01-09 / %x0b-0c / %x0e-1f / %x21-ff`<br id="nolfcrsp">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value except:
	>	- 0x00 (`\0`)
	>	- 0x0a (`\n`)
	>	- 0x0d (`\r`)
	>	- 0x20 (` `)
	</details>
	<!-- #endregion -->
	<br>
- nolfcrspcl = `%x01-09 / %x0b-0c / %x0e-1f / %x21-39 / %x3b-ff`<br id="nolfcrspcl">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 octet that has any value except:
	>	- 0x00 (`\0`)
	>	- 0x0a (`\n`)
	>	- 0x0d (`\r`)
	>	- 0x20 (` `)
	>	- 0x3a (`:`)
	</details>
	<!-- #endregion -->
	<br>
- nolfcrspat = `%x01-09 / %x0b-0c / %x0e-1f / %x21-3f / %x41-ff`<br id="nolfcrspat">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>
	
	>	1 octet that has any value except:
	>	- 0x00 (`\0`)
	>	- 0x0a (`\n`)
	>	- 0x0d (`\r`)
	>	- 0x20 (` `)
	>	- 0x40 (`@`)
	</details>
	<!-- #endregion -->
	<br>
- year = `1*digit`<br id="year">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1 or more [digits](#digit)
	</details>
	<!-- #endregion -->
	<br>
- month = `%x30 %x31-39 / %x31 %x30-32`<br id="month">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	Either 1 of the following:
	>	- 1 octet that has the value 0x30 (`0`)<br>
	>		followed by 1 octet that has any value<br>
	>		between 0x31 (`1`) and 0x39 (`9`) both included
	>	- 1 octet that has the value 0x31 (`1`)<br>
	>		followed by 1 octet that has any value<br>
	>		between 0x30 (`0`) and 0x32 (`2`) both included
	</details>
	<!-- #endregion -->
	<br>
- day = `%x30 %x31-39 / %x31-32 %x30-39 / %x33 %x30-31`<br id="day">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	Either 1 of the following:
	>	- 1 octet that has the value 0x30 (`0`)<br>
	>		followed by 1 octet that has any value<br>
	>		between 0x31 (`1`) and 0x39 (`9`) both included
	>	- 1 octet that has either the value 0x31 (`1`) or the value 0x32 (`2`)<br>
	>		followed by 1 octet that has any value<br>
	>		between 0x30 (`0`) and 0x39 (`9`) both included
	>	- 1 octet that has the value 0x33 (`3`)<br>
	>		followed by 1 octet that has any value<br>
	>		between 0x30 (`0`) and 0x31 (`1`) both included
	</details>
	<!-- #endregion -->
	<br>
- shortname = `( letter / digit ) *( letter / digit / %x2d )`<br id="shortname">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 octet that is either a [letter](#letter) or a [digit](#digit)
	>	1. Contain 0 or more octets that are [letters](#letter) and/or [digits](#digit)<br>
	>	and/or have the value 0x2d (`-`)
	</details>
	<!-- #endregion -->
	<br>
- channelid = `5( uppercase / digit )`<br id="channelid">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	5 octets that are either [uppercases](#uppercase) and/or [digits](#digit)
	</details>
	<!-- #endregion -->
	<br>
- chanstring = `*( %x01-06 / %x08-09 / %x0b-0c / %x0e-1f / %x21-2b / %x2d-39 / %x3b-ff )`<br id="chanstring">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	0 or more octets that have any value except:
	>		- 0x00 (`\0`)
	>		- 0x07 (`\a`)
	>		- 0x0a (`\n`)
	>		- 0x0d (`\r`)
	>		- 0x20 (` `)
	>		- 0x2c (`,`)
	>		- 0x3a (`:`)
	</details>
	<!-- #endregion -->
	<br>
- middle = `nolfcrspcl *nolfcrsp`<br id="middle">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [nolfcrspcl](#nolfcrspcl)
	>	1. Contain 0 or more [nolfcrsp](#nolfcrsp)
	</details>
	<!-- #endregion -->
	<br>
- trailing = `*nolfcr`<br id="trailing">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	0 or more [nolfcr](#nolfcr)
	</details>
	<!-- #endregion -->
	<br>
- date = `year %x2d month %x2d day`<br id="date">
	<!-- #region: Explained ABNF notation -->
	<details>
	<summary>Explanations about the above notation</summary>

	>	1. Start with 1 [year](#year)
	>	1. Contain 1 octet that has the value 0x2d (`-`)
	>	1. Contain 1 [month](#month)
	>	1. Contain 1 octet that has the value 0x2d (`-`)
	>	1. Contain 1 [day](#day)
	</details>
	<!-- #endregion -->
