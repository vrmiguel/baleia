# github.com/vrmiguel/baleia

from telegram import ParseMode
from telegram.ext import Updater, CommandHandler
import logging
from subprocess import Popen, PIPE
try:
    import bot_details      # A local bot_details.py file.
except ModuleNotFoundError:
    raise Exception("A bot_details.py file is needed. Check the README if necessary.")

secrets = bot_details.Secrets

# Configuring logging
logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO)

# Makes an instance to updater, which employs dispatcher and provides a frontend to telegram.Bot
updater = Updater(token=secrets.telegram_token, use_context=True)

# Saves the instance to dispatcher used by updater, for easy access
dispatcher = updater.dispatcher

    ## Function definitions

def is_authorized(update):
    uid = update.message.from_user.id
    return uid in secrets.acceptable_uids

def start(update, context):
    if not is_authorized(update):
        logging.info(f"{update.message.from_user.id} ({update.message.from_user.username}) is unauthorized.")
        update.message.reply_text(f'User {update.message.from_user.full_name} is not authorized to access this bot.')
        return
    logging.info(f"in start: user's id: {update.message.from_user.id}")
    logging.info(f"in start: user's full name: {update.message.from_user.full_name}")
    logging.info(f"{update.message.from_user.username} used '\\start'")
    update.message.reply_text('Welcome to baleia-bot v.0.1-alpha')

def bot_help(update, context):
    update.message.reply_text("\t\\start - Initial bot message.\n\t\\get_log - Get the output of a Baleia run.")

def cancel(update, context):
    user = update.message.from_user.first_name
    logging.info("User %s canceled the conversation.", user)
    update.message.reply_text('User cancelled.')

def get_log(update, context):
    if not is_authorized(update):
        update.message.reply_text(f'User {update.message.from_user.full_name} is not authorized to access this bot.')
        return
    logging.info(f"{update.message.from_user.username} used '\\get_log'")
    subproc = Popen(["./baleia", "-a", "-d"], stdout=PIPE, stderr=PIPE, universal_newlines=True)
    output, _ = subproc.communicate()
    update.message.reply_text(f'```{output}```', parse_mode=ParseMode.MARKDOWN)
    logging.info("log shown")

def main():
    dispatcher.add_handler(CommandHandler('start', start))
    dispatcher.add_handler(CommandHandler('get_log', get_log))
    dispatcher.add_handler(CommandHandler('help', bot_help))

    # Starts the bot now!
    updater.start_polling()

    updater.idle()

if __name__ == '__main__':
    main()
