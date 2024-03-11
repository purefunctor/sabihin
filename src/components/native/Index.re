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
  </main>;
};
