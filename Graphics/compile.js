const fs = require("fs");
const puppeteer = require("puppeteer");

(async () => {
	const browser = await puppeteer.launch();
	const page = await browser.newPage();
	const url = "file:///" + __dirname.replace(/\\/g, '/') + "/textures.svg";

	await page._emulationManager._client.send(
		"Emulation.setDefaultBackgroundColorOverride",
		{ color: { r: 0, g: 0, b: 0, a: 0 } }
	);

	await page.setViewport({ width: 256, height: 256 });
	await page.goto(url);

	const widths = await page.evaluate(() => {
		var elements = document.querySelectorAll(".row > *");
		var result = [];

		for (let i = 11; i < 105; ++i) {
			result.push(elements[i].getBoundingClientRect().width + 4);
		}

		return result;
	});

	await page.screenshot({ path: "textures.png" });
	await browser.close();
	fs.writeFile("font.bin", new Buffer(widths), (error) => { console.log(error); });
})();
