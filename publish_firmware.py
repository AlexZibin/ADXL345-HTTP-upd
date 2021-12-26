# Copyright (c) 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import requests
import sys
import os
from os.path import basename

Import("env")

print('publish_firmware.py running...')

# get api token, as the "old way" does not seem to work with sysenv for windows 
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

url = defines.get("UPDATE_SERVER_URL")[1:-1] + "upload.php"
# print("______URL:________")
# print(url)
# print("__________________")

def publish_firmware(source, target, env):
    firmware_path = str(source[0])
    firmware_name = basename(firmware_path)

    print("Uploading {0} to our repository.".format(firmware_name))

    #url = "http://i..." + "upload.php"

    headers = {
        "Content-type": "application/octet-stream",
        "X-Mytray-key": "qndfrhgjkancjdrs"
    }

    r = None
    try:
        files = {'file': open(firmware_path, 'rb')}
        
        #use of headers leads to malfunction of requests.post(), so now we have no authentication at all (((
        r = requests.post(url, files=files)

        r.raise_for_status()
        print("py status code:", r.status_code)
        print("py text:", r.text)
    except requests.exceptions.RequestException as e:
        sys.stderr.write("Failed to submit package: %s\n" %
                         ("%s\n%s" % (r.status_code, r.text) if r else str(e)))

        print("ERR py status code:", r.status_code)
        print("ERR py text:", r.text)
        print(r.text)

        env.Exit(1)

    print("The firmware has been successfuly published!")


# Custom upload command 
env.Replace(UPLOADCMD=publish_firmware)