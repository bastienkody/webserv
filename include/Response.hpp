/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuesser <mmuesser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:48:03 by mmuesser          #+#    #+#             */
/*   Updated: 2024/06/13 16:07:39 by mmuesser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Response
{
	public:

		Response(void);
		~Response(void);

		std::string getLineState() const;
		std::string getHeader() const;
		std::string getBody() const;

		void	setLineState(std::string lineState);
		void	setHeader(std::string header);
		void	setBody(std::string body);

	private:

		std::string _lineState;
		std::string _header;
		std::string _body;
};