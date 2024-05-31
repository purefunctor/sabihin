let bannerCss = [%cx
  {|
padding-top: 4rem;
padding-bottom: 4rem;
font-family: "Poppins";
text-align: center;

$(Theme.afterMedium) {
  text-align: left;
}
|}
];

let titleCss = [%cx
  {|
font-size: 1.5rem;
margin-top: 0;
margin-bottom: 0;

$(Theme.afterMedium) {
  font-size: 4rem;
}
|}
];

let subtitleCss = [%cx
  {|
margin-top: 2rem;
margin-bottom: 0;

$(Theme.afterMedium) {
  font-size: 1.5rem;
}
|}
];

let titleUnderlineCss = [%cx
  {|
text-decoration: underline;
text-decoration-thickness: 0.1em;
text-decoration-color: $(Theme.primary);
text-underline-position: from-font;
|}
];

let actionButtonsCss = [%cx
  {|
margin-top: 2rem;
display: flex;
gap: 1rem;
justify-content: center;

$(Theme.afterMedium) {
  justify-content: left;
}
|}
];

let iconButton = [%cx {|
font-family: "Josefin Sans";
gap: 0.5rem;
|}];

[@react.component]
let make = () => {
  <section className=bannerCss>
    <header>
      <h1 className=titleCss>
        <div className=titleUnderlineCss>
          {React.string("End-to-End Encrypted")}
        </div>
        <div className=titleUnderlineCss>
          {React.string("Anonymous Confessions")}
        </div>
      </h1>
      <p role="doc-subtitle" className=subtitleCss>
        {React.string(
           "sabihin.ph is a privacy-first platform for anonymous confessions.",
         )}
      </p>
      <nav className=actionButtonsCss>
        <Link
          className={Theme.primaryButton ++ " " ++ iconButton}
          href="/register">
          <Icons.SparkleLine size="1.5rem" />
          <span> {React.string("Get Started")} </span>
        </Link>
        <Link className={Theme.ghostButton ++ " " ++ iconButton} href="/learn">
          <Icons.BookLine size="1.5rem" />
          <span> {React.string("Learn More")} </span>
        </Link>
      </nav>
    </header>
  </section>;
};
