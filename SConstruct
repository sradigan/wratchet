# SConstruct

#Variables
install_dir = './bin'

#environment
# Also sets up some variables
env = Environment()
env.Append( CPPPATH = ['include'] )
env.Append( CPPFLAGS = ['-Wall', '-Wextra'] )

#build library
wratchet_lib = env.Library('wratchet', ['src/socket.c'])


#install
env.Install(install_dir, wratchet_lib)
env.Alias('install', install_dir)
