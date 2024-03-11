module Section = {
  [@react.component]
  let make = (~title, ~children) => {
    <section className="footer-section">
      <header className="josefin-sans-title"> {React.string(title)} </header>
      children
    </section>;
  };
};

module FooterLink = {
  [@react.component]
  let make = (~href, ~text) => {
    <p> <a href className="footer-link"> {React.string(text)} </a> </p>;
  };
};

[@react.component]
let make = () => {
  <footer className="layout-footer">
    <Section title="sabihin.ph">
      <article className="josefin-sans-regular color-offwhite">
        <p> {React.string("anonymous and secure confessions")} </p>
        <p> {React.string("built with <3 in OCaml+Reason")} </p>
        <p> {React.string("(c) purefunctor 2024")} </p>
      </article>
    </Section>
    <Section title="links">
      <nav className="josefin-sans-regular">
        <FooterLink
          href="https://github.com/purefunctor/sabihin"
          text="GitHub"
        />
        <FooterLink href="https://purefunctor.me" text="Blog" />
        <FooterLink
          href="https://github.com/sponsors/purefunctor/"
          text="Sponsor Me!"
        />
      </nav>
    </Section>
    <Section title="special mentions">
      <nav className="josefin-sans-regular">
        <FooterLink href="https://ossph.org" text="OSSPH" />
      </nav>
    </Section>
    <Section title="more like this">
      <article className="josefin-sans-regular color-offwhite">
        <p> {React.string("nothing to see yet")} </p>
      </article>
    </Section>
  </footer>;
};
