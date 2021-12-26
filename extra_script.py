Import("env")
from pprint import pprint

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
print('extra_script.py prints all #defines from platformio.ini:')
pprint(defines)
print("BIN_PROGNAME: "+defines["BIN_PROGNAME"])
#print(defines.get("BIN_PROGNAME"))
env.Replace(PROGNAME="%s" % defines.get("BIN_PROGNAME")[1:-1])

