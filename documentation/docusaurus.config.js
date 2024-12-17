// @ts-check
// `@type` JSDoc annotations allow editor autocompletion and type checking
// (when paired with `@ts-check`).
// There are various equivalent ways to declare your Docusaurus config.
// See: https://docusaurus.io/docs/api/docusaurus-config

import { themes as prismThemes } from "prism-react-renderer";

/** @type {import('@docusaurus/types').Config} */
const config = {
  markdown: {
    mermaid: true,
  },
  themes: ["@docusaurus/theme-mermaid"],

  title: "Flatburn",
  tagline: `An open-source, solar-powered, and modular sensing device that can be easily deployed on road vehicles to collect environmental data.`,
  favicon: "img/favicon.ico",

  // Set the production url of your site here
  url: "https://mit-senseable-city-lab.github.io/",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/flatburn-lte/",
  trailingSlash: false,

  staticDirectories: ["static"],

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "MIT Senseable City Lab", // Usually your GitHub org/user name.
  projectName: "Flatburn", // Usually your repo name.

  onBrokenLinks: "warn",
  onBrokenMarkdownLinks: "warn",

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "en",
    locales: ["en"],
  },

  presets: [
    [
      "classic",
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          path: "content",
          sidebarPath: "./sidebars.js",
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl: "https://github.com/MIT-Senseable-City-Lab/flatburn-lte/tree/main/documentation",
        },
        // blog: {
        //   showReadingTime: true,
        //   // Please change this to your repo.
        //   // Remove this to remove the "edit this page" links.
        //   editUrl:
        //     'https://github.com/facebook/docusaurus/tree/main/packages/create-docusaurus/templates/shared/',
        // },
        theme: {
          customCss: "./src/css/custom.css",
        },
      }),
    ],
  ],
  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      mermaid: {
        theme: { light: "neutral", dark: "dark" },
      },
      // Replace with your project's social card
      image: "img/docusaurus-social-card.jpg",
      navbar: {
        // title: "Flatburn",
        logo: {
          alt: "MIT Senseable City Lab",
          src: "img/logo.svg",
        },
        items: [
          // {
          //   type: "search",
          //   position: "left",
          // },
          {
            type: "docSidebar",
            sidebarId: "tutorialSidebar",
            position: "left",
            label: "Flatburn",
            className: "project-name-label",
          },
          // { to: "/blog", label: "Blog", position: "left" },
          {
            href: "https://github.com/MIT-Senseable-City-Lab/flatburn-lte/",
            // label: "GitHub",
            position: "right",
            className: "navbar-social-link navbar-github-logo",
            "aria-label": "GitHub repository",
          },
        ],
      },
      footer: {
        style: "light",
        links: [
          {
            title: "Docs",
            items: [
              {
                label: "Get started",
                to: "/docs/about",
              },
            ],
          },
          {
            title: "Follow us",
            items: [
              {
                label: "Website",
                href: "https://senseable.mit.edu",
              },
              {
                label: "Instagram",
                href: "https://www.instagram.com/senseable_city_lab/",
              },
              {
                label: "Twitter",
                href: "https://twitter.com/SenseableCity",
              },
            ],
          },
          {
            title: " ",
            items: [
              // {
              //   label: 'Blog',
              //   to: '/blog',
              // },
              {
                label: "GitHub",
                href: "https://github.com/MIT-Senseable-City-Lab",
              },
              {
                label: "YouTube",
                href: "https://www.youtube.com/user/senseablecitylab",
              },
              {
                label: "LinkedIn",
                href: "https://www.linkedin.com/company/mit-senseable-city-lab/",
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} MIT Senseable City Lab.`,
      },
      prism: {
        theme: prismThemes.github,
        darkTheme: prismThemes.dracula,
      },
    }),
};

export default config;
