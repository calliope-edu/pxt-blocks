// tests go here; this will not be compiled when this package is used as an extension.
Blocks.onReceivedTextWithLabel("label-01", function (content) {
  Blocks.sendTextWithLabel("label-01", content);
  // basic.showString("label-01 = T " + (content))
})
Blocks.onReceivedNumberWithLabel("label-01", function (content) {
  Blocks.sendNumberWithLabel("label-01", content);
  // basic.showString("label-01 = N " + (content))
})
Blocks.startService()
