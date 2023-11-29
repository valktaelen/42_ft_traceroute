# 42_ft_traceroute
This project will make you recode the traceroute command

## Usage

### Compilation

`make` or `make bonus`


### Execute

*Warning*, to execute we need to priviledge user (for open socket).

```bash
sudo ./ft_traceroute [args ...]
```

```
Usage
ft_traceroute [options] <destination>

Options:
        <destination>   dns name or ip address
        -h              help
        -f              first_ttl <integer> default 1
        -m              max_ttl <integer> default 30
        -q              nqueries <integer> Sets the number of probe packets per hop. The default is 3
        -z              sendwait <integer> Minimal  time  interval between probes (default 0).
        -n              Do not try to map IP addresses to host names when displaying them
        -4              ipv4 only
```

## How it works

It prints all the routers address for reach the final host (route of the packet).

To do traceroute, we need ICMP ([Internet Control Message Protocol](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol)), which is a network layer protocol that is an integral part of the Internet Protocol (IP) suite.
The main purpose of using ICMP in the traceroute command is to check the reachability of a destination host. By sending ICMP Echo Request messages, the source host can determine if the destination host is responsive.

- Packet Routing:
	- Traceroute works by sending a sequence of packets, each with an increasing Time to Live (TTL) value.
	- The TTL is a field in the IP header that limits the lifespan of a packet. Each router along the path decreases the TTL by one.

- ICMP Time Exceeded Message:
	- When a packet's TTL reaches zero, the router handling the packet discards it and sends an ICMP Time Exceeded message back to the source.
	- This ICMP Time Exceeded message indicates that the packet has been discarded due to the TTL reaching zero and includes information about the router that discarded the packet.

- Tracing the Route:
	- By sending packets with increasing TTL values, Traceroute can effectively trace the route from the source to the destination.
	- The first set of packets will reach the first router along the path, and its TTL will expire, generating an ICMP Time Exceeded message. Traceroute notes the IP address of the first router.
	- The next set of packets will have a TTL that allows them to reach the second router, and so on.

- Destination Reached:
	- Eventually, a packet will reach the destination host, and it will respond with an ICMP Echo Reply.
	- Traceroute identifies that the destination has been reached, and the tracing process concludes.

- Displaying Results:
	- Traceroute displays the route by listing the IP addresses or hostnames of the routers or devices along the path.
	- Additionally, it provides round-trip times for each hop and may resolve IP addresses to hostnames if possible.
