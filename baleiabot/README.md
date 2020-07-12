# baleiabot

*⚠️ Super early work in progress.*

## Commands

```shell
    */start    - Starts the bot
    */get_log  - Prints out a full Baleia log
    */help     - Prints out a small help message
```

## Usage

Dependencies:
* Python 3.6+
* [python-telegram-bot](https://github.com/python-telegram-bot/python-telegram-bot)

Requires a `bot_details.py` file with a Secrets class (shown below) and a Baleia binary in the same directory the bot is running on.

The `bot_details.py` file should store the following info:

```python
class Secrets:
    acceptable_uids = [123456789, 234567890]            # The unique ID of the users authorized to use the bot.
    telegram_token = "1234567890:mAnYmAnYcHaRacTeRs"    # Your bot's Token
```
