from __future__ import print_function
import datetime
import time
from googleapiclient.discovery import build
from httplib2 import Http
from oauth2client import file, client, tools
import PyCmdMessenger


SCOPES = 'https://www.googleapis.com/auth/calendar.readonly'

# Connecting to arduino
arduino = PyCmdMessenger.ArduinoBoard("COM19", baud_rate=9600)  # if using a Rasperry Pi change "COM19" to /dev/ttyUSB0
commands = [["error", "s"],
            ["send_starttime", "l"],
            ["send_endtime", "l"],
            ["clear_array", "i"],
            ["s", "i"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

def main():
    store = file.Storage('token.json')
    creds = store.get()
    if not creds or creds.invalid:
        flow = client.flow_from_clientsecrets('credentials.json', SCOPES)
        creds = tools.run_flow(flow, store)
    service = build('calendar', 'v3', http=creds.authorize(Http()))

    # Call the Calendar API
    today = datetime.datetime.now()  # - datetime.timedelta(days=1)
    today_start = datetime.datetime(today.year, today.month, today.day, 0, 0, 0, 0).isoformat() + 'Z'
    today_end = datetime.datetime(today.year, today.month, today.day, 23, 59, 59, 999).isoformat() + 'Z'

    print('Getting the upcoming events')
    events_result = service.events().list(calendarId='primary', timeMin=today_start,
                                          timeMax=today_end, singleEvents=True,
                                          orderBy='startTime').execute()
    events = events_result.get('items', [])

    c.send("clear_array", 1)

    if not events:
        print('No upcoming events found.')
    for event in events:
        start = event['start'].get('dateTime', event['start'].get('date'))
        end = event['end'].get('dateTime', event['end'].get('date'))
        print(start, end, event['summary'])

        start2 = start.split("T")
        starttime = start2[1]
        starttime2 = starttime.split("+")
        starttime3 = starttime2[0]
        print(starttime3)
        (h, m, s) = starttime3.split(':')
        starttimeint = int(h) * 3600 + int(m) * 60 + int(s)
        print(starttimeint)

        c.send("send_starttime", starttimeint)
        time.sleep(1)

        end2 = end.split("T")
        endtime = end2[1]
        endtime2 = endtime.split("+")
        endtime3 = endtime2[0]
        print(endtime3)
        (h, m, s) = endtime3.split(':')
        endtimeint = int(h) * 3600 + int(m) * 60 + int(s)
        print(endtimeint)

        c.send("send_endtime", endtimeint)
        time.sleep(1)

def get_time():
    currentDate = time.strftime('%H:%M:%S')
    print(currentDate)
    (h, m, s) = currentDate.split(":")
    currentSec = int(h) * 3600 + int(m) * 60 + int(s)
    currentKwart = currentSec / 900
    currentint = int(currentKwart)
    print(currentint)
    c.send("s", currentint)


while True:
    main()
    time.sleep(1)
    get_time()
    time.sleep(10)

