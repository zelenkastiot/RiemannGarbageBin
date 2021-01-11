# Riemann Garbage Bin (RGB) model
[**Filip Karafiloski**](https://www.researchgate.net/profile/Filip_Karafiloski) | [**Igor Mishkovski**](https://www.researchgate.net/profile/Igor_Mishkovski) | [**Kiril Zelenkovski**](https://www.researchgate.net/profile/Kiril_Zelenkovski) 


The paper was published to a local conference ([CIIT](http://ciit.finki.ukim.mk/)) in 2018. <br> 
The preprint is avilable here: *["Riemann Garbage Bin: A Self-sustained Waste Management System"](https://www.researchgate.net/publication/333058906_Riemann_Garbage_Bin_A_Self-sustained_Waste_Management_System)*. 

## Abstract 
This repo contains all of the codes for the prototype explained in the paper. I wrote it during my 2nd year of Computer Science studies and the idea for the garbage bin came to me while solving a Calculus exam question. The paper shortly elaborates the idea (it has some issues) for building a self-sustained sensor system, a hypothetical network composed of nodes implementing the Riemann Garbage Bin (RGB) model. The model is named after the creator of the formula, Riemann Sum approximaties an integral by a finite sum.

<p align="center">
<img src="https://raw.githubusercontent.com/zelenelez/images/master/model.png" width=45%;></img>
</p>

Hence, the case study explores the potential of employing sensor-enabled systems to improve waste monitoring and management in public waste bins. The network consists of wireless nodes that use ultrasonic sensors to measure the space in the bins. The sensors periodically report the fill rate of the waste bins to a sensor gateway that is based on the Long Range Wide Area Network (LoRaWAN) protocol. For our LoRaWAN server and network cover, we choose to use The Things Network (TTN). These fill rates would be monitored by a mobile or web application connected to the network server. The goal of this project is through the Internet of Things (IoT) to monitor all the waste bins in one city, to improve garbage management by relocating resources, and by giving insight to the public about this global health threatening problem.


Prototype 2[25.03.2019]: https://youtu.be/gkhP9VHziCQ

<hr>
<p align="center">
<img src="https://raw.githubusercontent.com/zelenelez/images/master/finki.jpg" width=70%;></img> <br>
Spring, 2018
</p>
