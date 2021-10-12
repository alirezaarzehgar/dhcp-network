# dhcp-network

Network module should handle every function that iteract with socket and network.

## Dependencies

- [dhcp-pkt](https://github.com/alirezaarzehgar/dhcp-pkt.git)

## Tasks

- <a href="#listener">listener</a>
- <a href="#network_utils">network utils</a>
  - <a href="#arp">arp</a>
  - <a href="#ping">ping</a>

### <p name="listener"> listener </p>

In DHCP we need a socket listener.

Actually everything starts from our listener and our listener is our endpoint on network.

This section listen on port 67 (UDP) and prepare a platform for iteracting with network.

common validations like arping mac addresses and pinging ip addresses applied in this layer.

We get a callback from parameters and get information from this.

Listener implementation model :

```c
listener (callback_t callback)
{
    /* `TODO Recive Discovery */

    /* `TODO Check requested ip address with ping */

    suitableIpAddressForLease = callback(discovery);

    /* `TODO send offer with `suitableIpAddressForLease` informations */

    /* `TODO recive request and compare with discover */

    /* `TODO if everything is OK, send ack */

    /* `TODO checking arp for dhcp starvation preventation */
}
```

## <p name="network_utils"> network utils </p>

## <p name="arp"> arp </p>

We need arp checking after leasing for Dhcp Starvation preventation.

## <p name="ping"> ping </p>

When we want to lease a ip address fist of all we should check its.

If this ip already sets statically we should create a lease from this ip and else we should skip this ip and choose next.

## Running tests

You should clone this repository with its dependencies

```
git clone https://github.com/alirezaarzehgar/dhcp-network.git --recurse-submodules
```

```
mkdir build
cd build
cmake ..
cmake --build .
sudo ./build/tests/dhcp-network-tests
```
