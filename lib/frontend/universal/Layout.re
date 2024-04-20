let headerRouterCss = [%cx {|
flex-grow: 1;
min-height: 100vh;
|}];

[@react.component]
let make = () => {
  <>
    <div className=headerRouterCss> <Header /> <Router /> </div>
    <Footer />
  </>;
};
