let indexPageCss = [%cx {|
margin: 2rem;
|}];

[@react.component]
let make = () => {
  <main className=indexPageCss>
    <IndexPageBanner />
    <IndexPageCards />
  </main>;
};
