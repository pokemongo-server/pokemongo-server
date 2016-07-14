# pokemongo-server

Nginx configuration
---

This server is using FastCGI.

Here's the nginx config:

```
server {
	listen 80;
	// ...............

	location /plfe/rpc {
		include /etc/nginx/fastcgi_params;
		fastcgi_pass    127.0.0.1:9100;
	}

	location /plfe/101/rpc {
		include /etc/nginx/fastcgi_params;
		fastcgi_pass    127.0.0.1:9000;
	}
}
```

Server configuration || how to compile
---

Dependencies: protobuf, fastcgi, fastcgi++ 

`make load_balancer` build the load balancer.

`make run_load_balancer` to start it.

`make server` to build the server

`make run_server` to start it.

Check if it works using your browser (http://127.0.0.1/plfe/rpc and http://127.0.0.1/plfe/101/rpc)


Mitmproxy "configuration"
---

Read here how to add the mitmproxy certificate to your android device:
https://blog.heckel.xyz/2013/07/01/how-to-use-mitmproxy-to-read-and-modify-https-traffic-of-your-phone/

Iptables rules (intercept only data to pgorelease.nianticlabs.com):

```
sysctl -w net.ipv4.ip_forward=1
iptables -t nat -A PREROUTING -i eth0 -p tcp --dport 443 -d pgorelease.nianticlabs.com -j REDIRECT --to-port 8080
```

And then `mitmproxy -T --host` to intercept data (without use this server)
or `mitmproxy -R http://127.0.0.1 --host` to redirect to local server.

