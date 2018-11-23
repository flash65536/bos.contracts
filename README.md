# bos.contracts

## BOSCore Version : v6.0.1
### EOSIO Contracts Version: v1.5.0

The design of the EOSIO blockchain calls for a number of smart contracts that are run at a privileged permission level in order to support functions such as block producer registration and voting, token staking for CPU and network bandwidth, RAM purchasing, multi-sig, etc.  These smart contracts are referred to as the system, token, msig and wrap (formerly known as sudo) contracts.

This repository contains examples of these privileged contracts that are useful when deploying, managing, and/or using an EOSIO blockchain.  They are provided for reference purposes:

   * [eosio.system](https://github.com/boscore/bos.contracts/tree/master/eosio.system)
   * [eosio.msig](https://github.com/boscore/bos.contracts/tree/master/eosio.msig)
   * [eosio.wrap](https://github.com/boscore/bos.contracts/tree/master/eosio.wrap)

The following unprivileged contract(s) are also part of the system.
   * [eosio.token](https://github.com/boscore/bos.contracts/tree/master/eosio.token)

Dependencies:
* [bos v6.0.x](https://github.com/boscore/bos/releases/tag/v6.0.1)
* [bos.cdt v6.0.x](https://github.com/boscore/bos.cdt/releases/tag/v6.0.1)

To build the contracts and the unit tests:
* First, ensure that your __eosio__ is compiled to the core symbol for the EOSIO blockchain that intend to deploy to.
* Second, make sure that you have ```sudo make install```ed __eosio__.
* Then just run the ```build.sh``` in the top directory to build all the contracts and the unit tests for these contracts.

After build:
* The unit tests executable is placed in the _build/tests_ and is named __unit_test__.
* The contracts are built into a _bin/\<contract name\>_ folder in their respective directories.
* Finally, simply use __cleos__ to _set contract_ by pointing to the previously mentioned directory.
