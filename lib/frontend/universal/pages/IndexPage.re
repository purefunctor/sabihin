let indexPageCss = [%cx {|
padding-top: 2rem;
padding-bottom: 2rem;
|}];

[@react.component]
let make = () => {
  <main className=indexPageCss>
    <IndexPageBanner />
    <IndexPageCards />
  </main>;
};
