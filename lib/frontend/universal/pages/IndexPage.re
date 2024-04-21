let indexPageCss = [%cx {|
padding-top: 2rem;
padding-bottom: 2rem;

$(Theme.afterMedium) {
  padding-top: 6rem;
}

$(Theme.afterLarge) {
  padding-top: 8rem;
}
|}];

[@react.component]
let make = () => {
  <main className=indexPageCss>
    <IndexPageBanner />
    <IndexPageCards />
  </main>;
};
