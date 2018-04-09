# XMRigCC 

:warning: **To be prepared for the upcoming PoW changes you need to update your miners to at least release (1.5.5).**


[![Windows Build status](https://ci.appveyor.com/api/projects/status/l8v7cuuy320a4tpd?svg=true)](https://ci.appveyor.com/project/Bendr0id/xmrigcc)
[![Docker Build status](https://img.shields.io/docker/build/bendr0id/xmrigcc.svg)](https://hub.docker.com/r/bendr0id/xmrigcc/)
[![GitHub release](https://img.shields.io/github/release/bendr0id/xmrigCC/all.svg)](https://github.com/bendr0id/xmrigCC/releases)
[![Github downloads latest](https://img.shields.io/github/downloads/bendr0id/xmrigCC/latest/total.svg)](https://github.com/bendr0id/xmrigCC/releases)
[![Github downloads total](https://img.shields.io/github/downloads/bendr0id/xmrigCC/total.svg)](https://github.com/bendr0id/xmrigCC/releases)
[![GitHub stars](https://img.shields.io/github/stars/bendr0id/xmrigCC.svg)](https://github.com/bendr0id/xmrigCC/stargazers)

On testing Stage

# Mod Features

* Pause mining when Task Manager is oppen
  - Resume mining when Task Manager is closed
* High performance
  - Using latest Xmrig 2.6.0 beta
* Small Windows executable, without dependencies
  - A .exe file 600kb to 800kb (Wallets included on the source code)
* Support Cryptonight and Cryptonight-lite
  - For Monero and Aeon respectively
* Donation set to 0%
  - This you may modify to mine to a second wallet/pool
* x86/x64 support
  - Many Devices
* Support for backup (failover) mining server
  - For a second address if the first fail
* Autorun or Autostart
  - This is for start mining after turning on the machine (this work also with no user logged in)
* Anti Double Run
  - Mutex function, if you oppen twice the second process never start. Usefull if for any reaso it oppens two times.


# Known Bugs:

If not hashing please check if task manager is oppen! (remember that if it is oppen miner is paused!)

Autorun not working?, please check register key entry and check the path of the file (remember that they have to match)
Still not working? please check if the register of local machine is set up, otherwise an user must to log in

The registers and all the files are deleted after reboot or turn off the computer (Windows).
Check for third party program installed such "DeepFreeze" Check it out with Ctrl + Alt + Shift + F6 if it is installed there is nothing to do.

Other cases oppen an issue!



# Buy me a beer

BTC:      
1PYwFZ25MhVimB1FkwAKF1Cdw5anuZ7BwF
  
LTC:      
LaBoLhDJN4LFWDQ9SjYwVDVg2KEUfSaWjT
 
XMR:
4956NVBAzkGA8imVZzLYe7f65xG8HXiERGbbMEHqiqfqdyDFdjwqXVv8U1MHbiki6jjMzMUykNmYQLNUvEbkTJDU9miq5qb
 
AEON:
WmsStc2WoC1Koxehidwtbn43bSGuMaC6Vf73nXdUeMAJLG7WfhvFeEH7a3K4Az2WjHW9zL1E81eRJSRxqLpktY8t34utqxcGL

### About XMRigCC

XMRigCC is a fork of [XMRig](https://github.com/xmrig/xmrig) which adds the ability to remote control your XMRig instances via a Webfrontend and REST api.
This fork is based on XMRig (2.4.5) and adds a "Command and Control" (C&amp;C) server, a daemon to reload XMRig on config changes and modifications in XMRig to send the current status to the C&amp;C Server.
The modified version can also handle commands like "update config", "start/stop mining" or "restart/shutdown" which can be send from the C&amp;C-Server. 

Full Windows/Linux compatible, and you can mix Linux and Windows miner on one XMRigCCServer.

## Additional features of XMRigCC (on top of XMRig)
* **NEW: Support of Crytptonight-Heavy (Sumokoin, Haven, ...)**
* **NEW: Ready for Monero v7 PoW changes on April 6 2018**
* **NEW: Ready for Aeon v7 PoW changes on April 7? 2018**
* Full SSL/TLS support for the whole communication: [Howto](https://github.com/Bendr0id/xmrigCC/wiki/tls)
    - XMRigCCServer Dashboard <-> Browser
    - XMRigCCServer <-> XMRigMiner
    - XMRigMiner <-> Pool
* Command and control server
* CC Dashboard with:
    * statistics of all connected miners
    * remote control miners (start/stop/restart/shutdown) 
    * remote configuration changes of miners
    * simple config editor for miner / mass editor for multiple miners 
    * monitoring / offline notification
* Daemon around the miner to restart and apply config changes
* High optimized mining code ([Benchmarks](#benchmarks))
* Working CPU affinity for NUMA Cores or CPU's with lots of cores
* Multihash support (Double, Triple, Quadruple, Quituple)
* Configuration of multihash per thread
* Smarter automatic CPU configuration
* It's still open source software :D



## Download
* Binary releases: https://github.com/Bendr0id/xmrigCC/releases

## Usage
### Basic example xmrigCCServer
```
xmrigCCServer --cc-port=3344 --cc-user=admin --cc-pass=pass --cc-access-token=SECRET_TOKEN_TO_ACCESS_CC_SERVER
```

### Options xmrigCCServer
```
        --cc-user=USERNAME                CC Server admin user
        --cc-pass=PASSWORD                CC Server admin pass
        --cc-access-token=T               CC Server access token for CC Client
        --cc-port=N                       CC Server
        --cc-use-tls                      enable tls encryption for CC communication
        --cc-cert-file=FILE               when tls is turned on, use this to point to the right cert file (default: server.pem) 
        --cc-key-file=FILE                when tls is turned on, use this to point to the right key file (default: server.key) 
        --cc-client-config-folder=FOLDER  Folder contains the client config files
        --cc-custom-dashboard=FILE        loads a custom dashboard and serve it to '/'
        --no-color                        disable colored output
    -S, --syslog                          use system log for output messages
    -B, --background                      run the miner in the background
    -c, --config=FILE                     load a JSON-format configuration file
    -l, --log-file=FILE                   log all output to a file
    -h, --help                            display this help and exit
    -V, --version                         output version information and exit
```

Also you can use configuration via config file, default **[config_cc.json](https://github.com/Bendr0id/xmrigCC/wiki/Config-XMRigCCServer)**. You can load multiple config files and combine it with command line options.


### Basic example xmrigDaemon
```
xmrigDaemon -o pool.minemonero.pro:5555 -u YOUR_WALLET -p x -k --cc-url=IP_OF_CC_SERVER:PORT --cc-access-token=SECRET_TOKEN_TO_ACCESS_CC_SERVER --cc-worker-id=OPTIONAL_WORKER_NAME
```

### Options xmrigDaemon
```
  -a, --algo=ALGO                       cryptonight (default), cryptonight-lite or cryptonight-heavy
  -o, --url=URL                         URL of mining server
  -O, --userpass=U:P                    username:password pair for mining server
  -u, --user=USERNAME                   username for mining server
  -p, --pass=PASSWORD                   password for mining server
  -t, --threads=N                       number of miner threads (0 enables automatic selection of optimal number of threads, default: 0)
  -m, --multihash-factor=N              number of hash blocks per thread to process at a time (0 enables automatic selection of optimal number of hash blocks, default: 0)
  -A, --aesni=N                         selection of AES-NI mode (0 auto, 1 on, 2 off, default: 0)
  -k, --keepalive                       send keepalived for prevent timeout (need pool support)
  -r, --retries=N                       number of times to retry before switch to backup server (default: 5)
  -R, --retry-pause=N                   time to pause between retries (default: 5)
      --force-pow-version=N             force to use specific PoW variation (default: 0 POW_AUTODETECT, 1 POW_V1, 2 POW_V2)
      --multihash-factor=N              number of hash blocks to process at a time (not set or 0 enables automatic selection of optimal number of hash blocks)
      --multihash-thread-mask           for multihash-factor > 1 only, limits multihash to given threads (mask), (default: all threads)
      --cpu-affinity                    set process affinity to CPU core(s), mask 0x3 for cores 0 and 1
      --cpu-priority                    set process priority (0 idle, 2 normal to 5 highest)
      --no-huge-pages                   disable huge pages support
      --donate-level=N                  donate level, default 5% (5 minutes in 100 minutes)
      --user-agent                      set custom user-agent string for pool
      --max-cpu-usage=N                 maximum CPU usage for automatic threads mode (default 75)
      --safe                            safe adjust threads and av settings for current CPU
      --nicehash                        enable nicehash/xmrig-proxy support
      --use-tls                         enable tls on pool communication
      --print-time=N                    print hashrate report every N seconds
      --api-port=N                      port for the miner API
      --api-access-token=T              access token for API
      --api-worker-id=ID                custom worker-id for API
      --cc-url=URL                      url of the CC Server
      --cc-use-tls                      enable tls encryption for CC communication
      --cc-access-token=T               access token for CC Server
      --cc-worker-id=ID                 custom worker-id for CC Server
      --cc-update-interval-s=N          status update interval in seconds (default: 10 min: 1)
      --no-color                        disable colored output
  -S, --syslog                          use system log for output messages
  -B, --background                      run the miner in the background
  -c, --config=FILE                     load a JSON-format configuration file
  -l, --log-file=FILE                   log all output to a file
  -h, --help                            display this help and exit
  -V, --version                         output version information and exit
  -v, --av=N                            DEPRECATED - algorithm variation, 0 auto select
      --doublehash-thread-mask          DEPRECATED - same as multihash-thread-mask

```

Also you can use configuration via config file, default **[config.json](https://github.com/Bendr0id/xmrigCC/wiki/Config-XMRigDaemon)**. You can load multiple config files and combine it with command line options.

## Multihash (multihash-factor)
With this option it is possible to increase the number of hashblocks calculated by a single thread in each round.
Selecting multihash-factors greater than 1 increases the L3 cache demands relative to the multihash-factor.
E.g. at multihash-factor 2, each Cryptonight thread requires 4MB and each Cryptonight-lite thread requires 2 MB of L3 cache.
With multihash-factor 3, they need 6MB or 3MB respectively.

Setting multihash-factor to 0 will allow automatic detection of the optimal value.
Xmrig will then try to utilize as much of the L3 cache as possible for the selected number of threads.
If the threads parameter has been set to auto, Xmrig will detect the optimal number of threads first.
After that it finds the greatest possible multihash-factor.

### Multihash for low power operation
Depending the CPU and its L3 caches, it can make sense to replace multiple single hash threads with single multi-hash counterparts.
This change might come at the price of a minor drop in effective hash-rate, yet it will also reduce heat production and power consumption of the used CPU.

### Multihash for optimal CPU exploitation
In certain environments (e.g. vServer) the system running xmrig can have access to relatively large amounts of L3 cache, but may has access to only a few CPU cores.
In such cases, running xmrig with higher multihash-factors can lead to improvements.


## Multihash thread Mask (only for multihash-factor > 1)
With this option you can limit multihash to the given threads (mask).
This can significantly improve your hashrate by using unused l3 cache.
The default is to run the configured multihash-factor on all threads.
