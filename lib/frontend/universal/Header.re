let headerCss = [%cx
  {|
display: flex;
align-items: center;

border-bottom: 1px solid $(Theme.backgroundSubtle);
margin: 2rem;
padding-bottom: 1rem;
|}
];

[@react.component]
let make = () => {
  <header className=headerCss> <HeaderTitle /> <HeaderGetStarted /> </header>;
};
