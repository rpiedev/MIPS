
const disconnectButton = document.getElementById("disconnectButton");
const colourPicker = document.getElementById("colourPicker");
const colourButton = document.getElementById("colourButton");

let devices;

function init() {
    const connectButton = document.getElementById("connectButton");
    navigator.usb.getDevices().then((devices) => {
        console.log(`Total devices: ${devices.length}`);
        devices.forEach((device) => {
            console.log(
                `Product name: ${device.productName}, serial number ${device.serialNumber}`,
            );
        });
    });
}