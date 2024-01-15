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
	- [Kick](#kick)
	- [Invite](#invite)
	- [Topic](#topic)
	- [Mode](#mode)
		- [Member Status](#member-status)
		- [Channel Flags](#channel-flags)
- [Numeric reply specifications](#numeric-reply-specifications)
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
The command part of the message must be either one of the following:
- a valid IRC command (see [Command specifications](#command-specifications) section)
- a numeric reply code (see [Numeric reply specifications](#numeric-reply-specifications) section)

Therefore, it must be formatted as follows:<br>
`1*letter / 3digit`

(click [here](#command-1) for more explanations about the above notation)

## Parameters
TODO

# Command specifications
## Kick
 Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
 [<comment>]

 The KICK command can be used to request the forced removal of a user
 from a channel. It causes the <user> to PART from the <channel> by
 force. For the message to be syntactically correct, there MUST be
 either one channel parameter and multiple user parameter, or as many
 channel parameters as there are user parameters. If a "comment" is
 given, this will be sent instead of the default message, the nickname
 of the user issuing the KICK.

 The server MUST NOT send KICK messages with multiple channels or
 users to clients. This is necessarily to maintain backward
 compatibility with old client software.

 Numeric Replies:
ERR_NEEDMOREPARAMS ERR_NOSUCHCHANNEL
ERR_BADCHANMASK ERR_CHANOPRIVSNEEDED
ERR_USERNOTINCHANNEL ERR_NOTONCHANNEL

 Examples:
 KICK &Melbourne Matthew ; Command to kick Matthew from
 &Melbourne
 KICK #Finnish John :Speaking English
 ; Command to kick John from #Finnish
 using "Speaking English" as the
 reason (comment).
 :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
 ; KICK message on channel #Finnish
 from WiZ to remove John from channel

 ## Invite
 Command: INVITE
 Parameters: <nickname> <channel>
 The INVITE command is used to invite a user to a channel. The
 parameter <nickname> is the nickname of the person to be invited to
 the target channel <channel>. There is no requirement that the
 channel the target user is being invited to must exist or be a valid
 channel. However, if the channel exists, only members of the channel
 are allowed to invite other users. When the channel has invite-only
 flag set, only channel operators may issue INVITE command.

 Only the user inviting and the user being invited will receive
 notification of the invitation. Other channel members are not
 notified. (This is unlike the MODE changes, and is occasionally the
 source of trouble for users.)

 Numeric Replies:
 ERR_NEEDMOREPARAMS ERR_NOSUCHNICK
 ERR_NOTONCHANNEL ERR_USERONCHANNEL
 ERR_CHANOPRIVSNEEDED
 RPL_INVITING RPL_AWAY

 Examples:
 :Angel!wings@irc.org INVITE Wiz #Dust
 ; Message to WiZ when he has been
 invited by user Angel to channel
 #Dust
 INVITE Wiz #Twilight_Zone ; Command to invite WiZ to
 #Twilight_zone

## Topic
Command: TOPIC
 Parameters: <channel> [ <topic> ]
 The TOPIC command is used to change or view the topic of a channel.
 The topic for channel <channel> is returned if there is no <topic>
 given. If the <topic> parameter is present, the topic for that
 channel will be changed, if this action is allowed for the user
 requesting it. If the <topic> parameter is an empty string, the
 topic for that channel will be removed.

 Numeric Replies:
 ERR_NEEDMOREPARAMS ERR_NOTONCHANNEL
 RPL_NOTOPIC RPL_TOPIC
 ERR_CHANOPRIVSNEEDED ERR_NOCHANMODES

 Examples:
 :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
 topic.
 TOPIC #test :another topic ; Command to set the topic on #test
 to "another topic".
 TOPIC #test : ; Command to clear the topic on
 #test.
 TOPIC #test ; Command to check the topic for
 #test.

## Mode
Channel Modes
 The various modes available for channels are as follows:
 o - give/take channel operator privilege;
 i - toggle the invite-only channel flag;
 t - toggle the topic settable by channel operator only flag;
 k - set/remove the channel key (password);
 l - set/remove the user limit to channel;
 
### Member Status
 The modes in this category take a channel member nickname as argument
 and affect the privileges given to this user.

Channel Operator Status
 The mode ’o’ is used to toggle the operator status of a channel
 member.

### Channel Flags
 The modes in this category are used to define properties which
 affects how channels operate.

Invite Only Flag
 When the channel flag ’i’ is set, new members are only accepted if
 their mask matches Invite-list (See section 4.3.2) or they have been
 invited by a channel operator. This flag also restricts the usage of
 the INVITE command (See "IRC Client Protocol" [IRC-CLIENT]) to
 channel operators.

Topic
 The channel flag ’t’ is used to restrict the usage of the TOPIC
 command to channel operators.

User Limit
 A user limit may be set on channels by using the channel flag ’l’.
 When the limit is reached, servers MUST forbid their local users to
 join the channel.
 The value of the limit MUST only be made available to the channel
 members in the reply sent by the server to a MODE query.

Channel Key
 When a channel key is set (by using the mode ’k’), servers MUST
 reject their local users request to join the channel unless this key
 is given.
 The channel key MUST only be made visible to the channel members in
 the reply sent by the server to a MODE query.



# Numeric reply specifications

# Explained ABNF notations
## Short name
1. Start with 1 character that is either a letter or a digit
1. Contain 0 or more characters that are letters and/or digits and/or dashes (`-`)

## Server name / Host
1. Start with 1 `shortname`, which must be formatted as described [here](#short-name)
1. Contain 0 or more patterns, which must:
	1. Start with 1 dot (`.`)
	1. Contain 1 `shortname`, which must be formatted as described [here](#short-name)

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
1. Contain 0 or 1 pattern, which must:
	1. Start with 1 colon (`:`)
	1. Contain a `chanstring`, which must be formatted as described [here](#channel-string)

## Messages
TODO: `[ ":" prefix space ] command [ params ] crlf`

## Prefix
1. Start with either one of the following:
	- `servername`, which must be formatted as described [here](#server-name--host)
	- 1 pattern, which must:
		1. Start with 1 `nickname`, which must be formatted as described [here](#user-nickname)
		1. Contain 0 or 1 pattern, which must:
			1. Start with 0 or 1 pattern, which must:
				1. Start with 1 exclamation mark (`!`)
				1. Contain 1 `user`, which must:
					1. Start with 1 or more characters that are any octet except:
						- NUL (`\0`)
						- LF (`\n`)
						- CR (`\r`)
						- space (` `)
						- at sign (`@`)
			1. Contain 1 at sign (`@`)
			1. Contain 1 `host`, which must be formatted as described [here](#server-name--host)

## Command
1. Start with either one of the following:
	- 1 or more letters
	- 3 digits


## Parameters
TODO
