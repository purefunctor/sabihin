module IndexCard = {
  [@react.component]
  let make = (~title, ~iconFn, ~children) => {
    <div className="feature-card poppins-bold">
      <article className="feature-card-article">
        <header> {React.string(title)} </header>
        children
      </article>
      {iconFn(~size="3rem", ~className="color-primary")}
    </div>;
  };
};

[@react.component]
let make = () => {
  <main className="index-content">
    <section className="index-banner">
      <header className="index-banner-header">
        <h2 className="poppins-bold">
          <span className="color-primary-gradient">
            {React.string("End-to-End Encrypted")}
          </span>
          <br />
          <span className="color-primary-gradient">
            {React.string("Anonymous Confessions")}
          </span>
        </h2>
        <p role="doc-subtitle" className="poppins-regular">
          {React.string(
             "sabihin.ph is a privacy-first platform for anonymous confessions.",
           )}
        </p>
        <nav className="index-banner-header-actions">
          <a className="button-primary icon-button josefin-sans-regular">
            <Icons.Sparkle size="1.25rem" />
            <span> {React.string("Get Started")} </span>
          </a>
          <a className="button-secondary-ghost icon-button josefin-sans-regular">
            <Icons.Book size="1.25rem" />
            <span> {React.string("Learn More")} </span>
          </a>
        </nav>
      </header>
    </section>
    <hr />
    <section className="index-cards" id="learn-more">
      <IndexCard
        title="End-to-End Encryption"
        iconFn={(~size, ~className) => <Icons.DoorLock size className />}>
        <p className="color-offwhite poppins-regular">
          {React.string("Sabihin utilizes end-to-end encryption techniques.")}
        </p>
        <p className="color-offwhite poppins-regular">
          {React.string(
             "Messages can only be read by its intended recipients.",
           )}
        </p>
      </IndexCard>
      <IndexCard
        title="Open-Source Software"
        iconFn={(~size, ~className) => <Icons.GitPr size className />}>
        <p className="color-offwhite poppins-regular">
          {React.string("Sabihin is open-source software.")}
        </p>
        <p className="color-offwhite poppins-regular">
          {React.string(
             "Its source code is public and contributions are welcome!",
           )}
        </p>
      </IndexCard>
      <IndexCard
        title="Functional Programming"
        iconFn={(~size, ~className) => <Icons.OCamlLogo size className />}>
        <p className="color-offwhite poppins-regular">
          {React.string(
             "Sabihin is built with functional programming languages.",
           )}
        </p>
        <p className="color-offwhite poppins-regular">
          {React.string(
             "OCaml and Reason offers performance, reliability, and blazingly-fast iteration times.",
           )}
        </p>
      </IndexCard>
    </section>
    // <hr />
  </main>;
};
